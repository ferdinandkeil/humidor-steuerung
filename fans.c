#include <avr/eeprom.h>
#include "lcd-routines.h"
#include "task.h"
#include "fans.h"
#include "menu.h"
#include "clock.h"
#include "sensor.h"

#define FAN_CTRL_OFF    0
#define FAN_CTRL_HUMI   1
#define FAN_CTRL_TIME   2
#define FAN_CTRL_REP    3

#define FAN_CTRL_MIN    0
#define FAN_CTRL_MAX    3
#define FAN_HUMI_MIN    1
#define FAN_HUMI_MAX   99
#define FAN_REP_MIN     1
#define FAN_REP_MAX    24
#define FAN_DUR_MIN     1
#define FAN_DUR_MAX    99

#define FAN_NR_OF_FANS  3

#define FAN_READOUT     0
#define FAN_EDIT        1

Fan_t Fan_fans[FAN_NR_OF_FANS] = {
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0}
};

const Fan_t Fan_fansE[FAN_NR_OF_FANS] EEMEM = {
	{FAN_CTRL_OFF, 72, 12, 00, 5, 12},
	{FAN_CTRL_OFF, 72, 12, 00, 5, 12},
	{FAN_CTRL_OFF, 72, 12, 00, 5, 12}
};

uint8_t Fan_edit;

#define Fan_getCtrl(fan) Fan_fans[fan].ctrl
#define Fan_setCtrl(fan, val) Fan_fans[fan].ctrl = val
#define Fan_getHumi(fan) Fan_fans[fan].humi
#define Fan_setHumi(fan, val) Fan_fans[fan].humi = val
#define Fan_getTimeH(fan) Fan_fans[fan].timeH
#define Fan_setTimeH(fan, val) Fan_fans[fan].timeH = val
#define Fan_getTimeM(fan) Fan_fans[fan].timeM
#define Fan_setTimeM(fan, val) Fan_fans[fan].timeM = val
#define Fan_getDur(fan) Fan_fans[fan].dur
#define Fan_setDur(fan, val) Fan_fans[fan].dur = val
#define Fan_getRep(fan) Fan_fans[fan].rep
#define Fan_setRep(fan, val) Fan_fans[fan].rep = val

// Zuordnung Funktionen

// Menü: Steuerung

int8_t Fan_State_1_Ctrl(int8_t input) {
	return Fan_State_Ctrl(0, input);
}

int8_t Fan_State_2_Ctrl(int8_t input) {
	return Fan_State_Ctrl(1, input);
}

int8_t Fan_State_3_Ctrl(int8_t input) {
	return Fan_State_Ctrl(2, input);
}

// Menü: Feuchtigkeit

int8_t Fan_State_1_Humi(int8_t input) {
	return Fan_State_Humi(0, input);
}

int8_t Fan_State_2_Humi(int8_t input) {
	return Fan_State_Humi(1, input);
}

int8_t Fan_State_3_Humi(int8_t input) {
	return Fan_State_Humi(2, input);
}

// Menü: Anschaltzeitpunkt

int8_t Fan_State_1_On(int8_t input) {
	return Fan_State_On(0, input);
}

int8_t Fan_State_2_On(int8_t input) {
	return Fan_State_On(1, input);
}

int8_t Fan_State_3_On(int8_t input) {
	return Fan_State_On(2, input);
}

// Menü: Intervall

int8_t Fan_State_1_Rep(int8_t input) {
	return Fan_State_Rep(0, input);
}

int8_t Fan_State_2_Rep(int8_t input) {
	return Fan_State_Rep(1, input);
}

int8_t Fan_State_3_Rep(int8_t input) {
	return Fan_State_Rep(2, input);
}

// Menü: Anschaltdauer

int8_t Fan_State_1_Dur(int8_t input) {
	return Fan_State_Dur(0, input);
}

int8_t Fan_State_2_Dur(int8_t input) {
	return Fan_State_Dur(1, input);
}

int8_t Fan_State_3_Dur(int8_t input) {
	return Fan_State_Dur(2, input);
}

// Menü: Zurück

int8_t Fan_State_1_Back(int8_t input) {
	return Fan_State_Back(0, input);
}

int8_t Fan_State_2_Back(int8_t input) {
	return Fan_State_Back(1, input);
}

int8_t Fan_State_3_Back(int8_t input) {
	return Fan_State_Back(2, input);
}

// Eigentliche Funktionen

int8_t Fan_State_Ctrl(uint8_t fan, int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		Fan_edit ^= 1;
		eeprom_write_byte((void*) &Fan_fansE[fan].ctrl, Fan_fans[fan].ctrl);
	}
	if (Fan_edit && (input == MENU_INPUT_DOWN)) {
		Fan_fans[fan].ctrl = (Fan_fans[fan].ctrl < FAN_CTRL_MAX) ? (Fan_fans[fan].ctrl+1) : (FAN_CTRL_MAX);
	}
	if (Fan_edit && (input == MENU_INPUT_UP)) {
		Fan_fans[fan].ctrl = (Fan_fans[fan].ctrl > FAN_CTRL_MIN) ? (Fan_fans[fan].ctrl-1) : (FAN_CTRL_MIN);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Fan_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_CTRL);
	switch (Fan_fans[fan].ctrl) {
		case FAN_CTRL_OFF:
			lcd_string_P(MENU_STR_CTRL_OFF);
			break;
		case FAN_CTRL_HUMI:
			lcd_string_P(MENU_STR_CTRL_HUMI);
			break;
		case FAN_CTRL_TIME:
			lcd_string_P(MENU_STR_CTRL_TIME);
			break;
		case FAN_CTRL_REP:
			lcd_string_P(MENU_STR_CTRL_REP);
			break;
	}
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_HUMIDITY);
	lcd_number(Fan_fans[fan].humi, 2, ' ');
	lcd_data('%');
	return Fan_edit;
}

int8_t Fan_State_Humi(uint8_t fan, int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		Fan_edit ^= 1;
		eeprom_write_byte((void*) &Fan_fansE[fan].humi, Fan_fans[fan].humi);
	}
	if (Fan_edit && (input == MENU_INPUT_DOWN)) {
		Fan_fans[fan].humi = (Fan_fans[fan].humi < FAN_HUMI_MAX) ? (Fan_fans[fan].humi+1) : (FAN_HUMI_MAX);
	}
	if (Fan_edit && (input == MENU_INPUT_UP)) {
		Fan_fans[fan].humi = (Fan_fans[fan].humi > FAN_HUMI_MIN) ? (Fan_fans[fan].humi-1) : (FAN_HUMI_MIN);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Fan_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_HUMIDITY);
	lcd_number(Fan_fans[fan].humi, 2, ' ');
	lcd_data('%');
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_ONTIME);
	lcd_number(Fan_fans[fan].timeH, 2, ' ');
	lcd_data(':');
	lcd_number(Fan_fans[fan].timeM, 2, '0');
	return Fan_edit;
}

int8_t Fan_State_On(uint8_t fan, int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		Fan_edit ^= 1;
		eeprom_write_byte((void*) &Fan_fansE[fan].timeM, Fan_fans[fan].timeM);
		eeprom_write_byte((void*) &Fan_fansE[fan].timeH, Fan_fans[fan].timeH);
	}
	if (Fan_edit && (input == MENU_INPUT_DOWN)) {
		if (Fan_fans[fan].timeM < 55) {
			Fan_fans[fan].timeM += 5;
		} else if (Fan_fans[fan].timeH < 23) {
			Fan_fans[fan].timeM = 0;
			Fan_fans[fan].timeH++;
		}
	}
	if (Fan_edit && (input == MENU_INPUT_UP)) {
		if (Fan_fans[fan].timeM > 0) {
			Fan_fans[fan].timeM -= 5;
		} else if (Fan_fans[fan].timeH > 0) {
			Fan_fans[fan].timeM = 55;
			Fan_fans[fan].timeH--;
		}
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Fan_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_ONTIME);
	lcd_number(Fan_fans[fan].timeH, 2, ' ');
	lcd_data(':');
	lcd_number(Fan_fans[fan].timeM, 2, '0');
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_REPEAT);
	lcd_number(Fan_fans[fan].rep, 2, ' ');
	lcd_data('h');
	return Fan_edit;
}

int8_t Fan_State_Rep(uint8_t fan, int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		Fan_edit ^= 1;
		eeprom_write_byte((void*) &Fan_fansE[fan].rep, Fan_fans[fan].rep);
	}
	if (Fan_edit && (input == MENU_INPUT_DOWN)) {
		Fan_fans[fan].rep = (Fan_fans[fan].rep < FAN_REP_MAX) ? (Fan_fans[fan].rep+1) : (FAN_REP_MAX);
	}
	if (Fan_edit && (input == MENU_INPUT_UP)) {
		Fan_fans[fan].rep = (Fan_fans[fan].rep > FAN_REP_MIN) ? (Fan_fans[fan].rep-1) : (FAN_REP_MIN);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Fan_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_REPEAT);
	lcd_number(Fan_fans[fan].rep, 2, ' ');
	lcd_data('h');
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_DURATION);
	lcd_number(Fan_fans[fan].dur, 2, ' ');
	lcd_string_P(MENU_STR_MIN);
	return Fan_edit;
}

int8_t Fan_State_Dur(uint8_t fan, int8_t input){
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		Fan_edit ^= 1;
		eeprom_write_byte((void*) &Fan_fansE[fan].dur, Fan_fans[fan].dur);
	}
	if (Fan_edit && (input == MENU_INPUT_DOWN)) {
		Fan_fans[fan].dur = (Fan_fans[fan].dur < FAN_DUR_MAX) ? (Fan_fans[fan].dur+1) : (FAN_DUR_MAX);
	}
	if (Fan_edit && (input == MENU_INPUT_UP)) {
		Fan_fans[fan].dur = (Fan_fans[fan].dur > FAN_DUR_MIN) ? (Fan_fans[fan].dur-1) : (FAN_DUR_MIN);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Fan_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_DURATION);
	lcd_number(Fan_fans[fan].dur, 2, ' ');
	lcd_string_P(MENU_STR_MIN);
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_BACK);
	return Fan_edit;
}

int8_t Fan_State_Back(uint8_t fan, int8_t input) {
	lcd_clear();
	lcd_home();
	lcd_data(' ');
	lcd_string_P(MENU_STR_DURATION);
	lcd_number(Fan_fans[fan].dur, 2, ' ');
	lcd_string_P(MENU_STR_MIN);
	lcd_setcursor(0, 2);
	lcd_data('>');
	lcd_string_P(MENU_STR_BACK);
	return 0;
}

#define Fan_turnOn(pin) PORTC |= (1<<pin)
#define Fan_turnOff(pin) PORTC &= ~(1<<pin)

/*
 Fan_checkFan(fan)
 Prüft ob Lüfter fan an- oder ausgeschaltet werden muss.
*/
static void Fan_checkFan(uint8_t fan) {
	// Lüfter zu Port zuordnen
	uint8_t pin;
	switch (fan) {
		default:
		case 0:
			pin = PC0;
			break;
		case 1:
			pin = PC2;
			break;
		case 2:
			pin = PC4;
			break;
	}
	if (Fan_fans[fan].ctrl == FAN_CTRL_HUMI) {
		// Steuerung: Feuchtigkeit
		if (Sensor_getHumi() < (Fan_fans[fan].humi*100)-1) {
			Fan_turnOn(pin);
		} else if (Sensor_getHumi() > (Fan_fans[fan].humi*100)+1) {
			Fan_turnOff(pin);
		} else {
			Fan_turnOff(pin);
		}
	} else if (Fan_fans[fan].ctrl == FAN_CTRL_TIME) {
		// Steuerung: Uhrzeit
		uint8_t end_m = Fan_fans[fan].timeM + Fan_fans[fan].dur;
		uint8_t end_h = Fan_fans[fan].timeH;
		// Überlauf Minuten bei Endzeit
		if (end_m >= 60) {
			end_h++;
			end_m -= 60;
			if (end_h > 23)
				end_h = 0;
			// Sonderfall: End-Stunde ungleich Start-Stunde
			if (((Clock_getHour() == Fan_fans[fan].timeH) && (Clock_getMinute() >= Fan_fans[fan].timeM))
				|| ((Clock_getHour() == end_h) && (Clock_getMinute() < end_m))) {
				Fan_turnOn(pin);
			} else {
				Fan_turnOff(pin);
			}
		} else {
			// End-Stunde und Start-Stunde gleich
			if ((Clock_getHour() == Fan_fans[fan].timeH) && (Clock_getMinute() >= Fan_fans[fan].timeM)
				&& (Clock_getMinute() < end_m)) {
				Fan_turnOn(pin);
			} else {
				Fan_turnOff(pin);
			}
		}
	} else if (Fan_fans[fan].ctrl == FAN_CTRL_REP) {
		// Steuerung: Intervall
		uint8_t end_m = Fan_fans[fan].dur - 1;
		if ((Clock_getHour() % Fan_fans[fan].rep == 0) && (Clock_getMinute() <= end_m)) {
			Fan_turnOn(pin);
		} else {
			Fan_turnOff(pin);
		}
	} else {
		// Alle andere Fälle: ausschalten
		Fan_turnOff(pin);
	}
}

static void Fan_run(void *_self, uint32_t now) {
	Fan_checkFan(0);
	Fan_checkFan(1);
	Fan_checkFan(2);
	Task_incRunTime(_self, 500);
}

Task_t* Fans(void) {
	eeprom_read_block(Fan_fans, Fan_fansE, FAN_NR_OF_FANS*sizeof(Fan_t));
	static Task_t fans;
	fans.canRun = TimedTask_canRun;
	fans.run = Fan_run;
	fans.runTime = 0;
	return &fans;
}