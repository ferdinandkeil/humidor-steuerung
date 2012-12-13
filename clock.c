#include "clock.h"
#include "menu.h"
#include "task.h"
#include "lcd-routines.h"

uint8_t Clock_s;
uint8_t Clock_m;
uint8_t Clock_h;

uint8_t Clock_edit;
uint8_t Clock_tmp;

int8_t Clock_State_Hour(int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		if (Clock_edit) {
			Clock_h = Clock_tmp;
			Clock_edit = 0;
		} else {
			Clock_tmp = Clock_h;
			Clock_edit = 1;
		}
	}
	if (Clock_edit && (input == MENU_INPUT_DOWN)) {
		Clock_tmp = (Clock_tmp < 23) ? (Clock_tmp+1) : (Clock_tmp);
	}
	if (Clock_edit && (input == MENU_INPUT_UP)) {
		Clock_tmp = (Clock_tmp > 0) ? (Clock_tmp-1) : (Clock_tmp);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Clock_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_HOUR);
	if (Clock_edit)
		lcd_number(Clock_tmp, 2, '0');
	else
		lcd_number(Clock_h, 2, '0');
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_MINUTES);
	lcd_number(Clock_m, 2, '0');
	return Clock_edit;
}

int8_t Clock_State_Minute(int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		if (Clock_edit) {
			Clock_m = Clock_tmp;
			Clock_s = 0;
			Clock_edit = 0;
		} else {
			Clock_tmp = Clock_m;
			Clock_edit = 1;
		}
	}
	if (Clock_edit && (input == MENU_INPUT_DOWN)) {
		Clock_tmp = (Clock_tmp < 59) ? (Clock_tmp+1) : (Clock_tmp);
	}
	if (Clock_edit && (input == MENU_INPUT_UP)) {
		Clock_tmp = (Clock_tmp > 0) ? (Clock_tmp-1) : (Clock_tmp);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Clock_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_MINUTES);
	if (Clock_edit)
		lcd_number(Clock_tmp, 2, '0');
	else
		lcd_number(Clock_m, 2, '0');
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_BACK);
	return Clock_edit;
}

int8_t Clock_State_Back(int8_t input) {
	lcd_clear();
	lcd_home();
	lcd_data(' ');
	lcd_string_P(MENU_STR_MINUTES);
	lcd_number(Clock_m, 2, '0');
	lcd_setcursor(0, 2);
	lcd_data('>');
	lcd_string_P(MENU_STR_BACK);
	return 0;
}


static void Clock_run(void *_self, uint32_t now) {
	static uint32_t last;
	static uint32_t accumulator;
	/*
	   Accumulator addiert die Differenz zwischen jetzt und
	   letzter Ausführung auf. Dadurch wird die Ausführung
	   zu unterschiedlichen Zeitpunkten ausgeglichen.
	   Bei Überlauf wird accumulator nicht auf null gesetzt,
	   um Abweichungen weiter aufaddieren zu können.
	*/
	accumulator += now - last;
	last = now;
	if (accumulator >= 1000) {
		accumulator -= 1000;
		Clock_s++;
		Menu_update(MENU_INPUT_CLOCK);
	}
	if (Clock_s >= 60) {
		Clock_s = 0;
		Clock_m++;
	}
	if (Clock_m >= 60) {
		Clock_m = 0;
		Clock_h++;
	}
	if (Clock_h >= 24) {
		Clock_h = 0;
	}
	Task_incRunTime(_self, 200);
}

Task_t* Clock(void) {
	Clock_s = 0;
	Clock_m = 0;
	Clock_h = 0;
	static Task_t clock;
	clock.canRun = TimedTask_canRun;
	clock.run = Clock_run;
	clock.runTime = 0;
	return &clock;
}