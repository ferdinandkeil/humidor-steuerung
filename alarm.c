#include <avr/eeprom.h>
#include "alarm.h"
#include "lcd-routines.h"
#include "menu.h"
#include "task.h"
#include "sensor.h"

#define ALARM_STATUS_ENABLE  1
#define ALARM_STATUS_LED     2
#define ALARM_STATUS_BUZZER  3

#define ALARM_HUMI_MIN       1
#define ALARM_HUMI_MAX      99

#define ALARM_PIN_BUZ      PD0
#define ALARM_PORT_BUZ   PORTD
#define ALARM_PIN_LED	   PC3
#define ALARM_PORT_LED	 PORTC

Alarm_t Alarm_settings = {0, 0};

Alarm_t Alarm_settingsE EEMEM = {(1<<ALARM_STATUS_LED), 68};

uint8_t Alarm_edit;

uint8_t Alarm_triggered;

int8_t Alarm_State_Enable(int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		Alarm_edit ^= 1;
		eeprom_write_byte((void*) &Alarm_settingsE.status, Alarm_settings.status);
	}
	if (Alarm_edit && (input == MENU_INPUT_DOWN)) {
		Alarm_settings.status |= (1<<ALARM_STATUS_ENABLE);
	}
	if (Alarm_edit && (input == MENU_INPUT_UP)) {
		Alarm_settings.status &= ~(1<<ALARM_STATUS_ENABLE);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Alarm_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_ALARM_ENABLE);
	if (Alarm_settings.status & (1<<ALARM_STATUS_ENABLE))
		lcd_string_P(MENU_STR_ON);
	else
		lcd_string_P(MENU_STR_OFF);
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_HUMIDITY);
	lcd_number(Alarm_settings.humidity, 2, ' ');
	lcd_data('%');
	return Alarm_edit;
}

int8_t Alarm_State_Humidity(int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		Alarm_edit ^= 1;
		eeprom_write_byte((void*) &Alarm_settingsE.humidity, Alarm_settings.humidity);
	}
	if (Alarm_edit && (input == MENU_INPUT_DOWN)) {
		Alarm_settings.humidity = (Alarm_settings.humidity < ALARM_HUMI_MAX) ? (Alarm_settings.humidity+1) : (ALARM_HUMI_MAX);
	}
	if (Alarm_edit && (input == MENU_INPUT_UP)) {
		Alarm_settings.humidity = (Alarm_settings.humidity > ALARM_HUMI_MIN) ? (Alarm_settings.humidity-1) : (ALARM_HUMI_MIN);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Alarm_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_HUMIDITY);
	lcd_number(Alarm_settings.humidity, 2, ' ');
	lcd_data('%');
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_LED);
	if (Alarm_settings.status & (1<<ALARM_STATUS_LED))
		lcd_string_P(MENU_STR_ON);
	else
		lcd_string_P(MENU_STR_OFF);
	return Alarm_edit;
}

int8_t Alarm_State_Led(int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		Alarm_edit ^= 1;
		eeprom_write_byte((void*) &Alarm_settingsE.status, Alarm_settings.status);
	}
	if (Alarm_edit && (input == MENU_INPUT_DOWN)) {
		Alarm_settings.status |= (1<<ALARM_STATUS_LED);
	}
	if (Alarm_edit && (input == MENU_INPUT_UP)) {
		Alarm_settings.status &= ~(1<<ALARM_STATUS_LED);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Alarm_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_LED);
	if (Alarm_settings.status & (1<<ALARM_STATUS_LED))
		lcd_string_P(MENU_STR_ON);
	else
		lcd_string_P(MENU_STR_OFF);
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_BUZZER);
	if (Alarm_settings.status & (1<<ALARM_STATUS_BUZZER))
		lcd_string_P(MENU_STR_ON);
	else
		lcd_string_P(MENU_STR_OFF);
	return Alarm_edit;
}

int8_t Alarm_State_Buzzer(int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		Alarm_edit ^= 1;
		eeprom_write_byte((void*) &Alarm_settingsE.status, Alarm_settings.status);
	}
	if (Alarm_edit && (input == MENU_INPUT_DOWN)) {
		Alarm_settings.status |= (1<<ALARM_STATUS_BUZZER);
	}
	if (Alarm_edit && (input == MENU_INPUT_UP)) {
		Alarm_settings.status &= ~(1<<ALARM_STATUS_BUZZER);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Alarm_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_BUZZER);
	if (Alarm_settings.status & (1<<ALARM_STATUS_BUZZER))
		lcd_string_P(MENU_STR_ON);
	else
		lcd_string_P(MENU_STR_OFF);
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_BACK);
	return Alarm_edit;
}

int8_t Alarm_State_Back(int8_t input) {
	lcd_clear();
	lcd_home();
	lcd_data(' ');
	lcd_string_P(MENU_STR_BUZZER);
	if (Alarm_settings.status & (1<<ALARM_STATUS_BUZZER))
		lcd_string_P(MENU_STR_ON);
	else
		lcd_string_P(MENU_STR_OFF);
	lcd_setcursor(0, 2);
	lcd_data('>');
	lcd_string_P(MENU_STR_BACK);
	return 0;
}
	
static void Alarm_run(void *_self, uint32_t now) {
	// Alarm auslösen?
	if ((Alarm_settings.status & (1<<ALARM_STATUS_ENABLE))
		&& (Sensor_getHumi() < Alarm_settings.humidity * 100)) {
		Alarm_triggered = 1;
	} else {
		Alarm_triggered = 0;
	}
	// LED
	static uint8_t led;
	if ((Alarm_settings.status & (1<<ALARM_STATUS_LED))
		&& Alarm_triggered) {
		if (led++ & 8)
			ALARM_PORT_LED ^= (1<<ALARM_PIN_LED);
	} else {
		ALARM_PORT_LED &= ~(1<<ALARM_PIN_LED);
	}
	// Piepser
	static uint8_t buz;
	if ((Alarm_settings.status & (1<<ALARM_STATUS_BUZZER))
		&& Alarm_triggered) {
		// Piepser Melodien 105 oder 233
		if (buz++ & 105)
			ALARM_PORT_BUZ &= ~(1<<ALARM_PIN_BUZ);
		else
			ALARM_PORT_BUZ |= (1<<ALARM_PIN_BUZ);
	} else {
		ALARM_PORT_BUZ &= ~(1<<ALARM_PIN_BUZ);
	}
	Task_incRunTime(_self, 50);
}

Task_t* Alarm(void) {
	eeprom_read_block(&Alarm_settings, &Alarm_settingsE, sizeof(Alarm_settings));
	Alarm_triggered = 0;
	static Task_t alarm;
	alarm.canRun = TimedTask_canRun;
	alarm.run = Alarm_run;
	alarm.runTime = 0;
	return &alarm;
}