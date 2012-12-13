#include <avr/eeprom.h>
#include "calibration.h"
#include "lcd-routines.h"
#include "menu.h"

int8_t Calibration_humidity;
int8_t Calibration_temperature;

uint8_t Calibration_humidityE EEMEM = 0;
uint8_t Calibration_temperatureE EEMEM = 0;

uint8_t Calibration_edit;

int8_t Calibration_State_Temp(int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		Calibration_edit ^= 1;
		eeprom_write_byte(&Calibration_temperatureE, Calibration_temperature);
	}
	if (Calibration_edit && (input == MENU_INPUT_DOWN)) {
		Calibration_temperature = (Calibration_temperature < 95) ? (Calibration_temperature+5) : (Calibration_temperature);
	}
	if (Calibration_edit && (input == MENU_INPUT_UP)) {
		Calibration_temperature = (Calibration_temperature > -95) ? (Calibration_temperature-5) : (Calibration_temperature);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Calibration_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_CAL_TEMP);
	int8_t tmp = Calibration_temperature;
	if (tmp & 0x80) {
		lcd_data('-');
		tmp = -tmp;
	} else {
		lcd_data('+');
	}
	lcd_data('0' + tmp / 10 % 10);
	lcd_data(',');
	lcd_data('0' + tmp % 10);
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_CAL_HUMI);
	tmp = Calibration_humidity;
	if (tmp & 0x80) {
		lcd_data('-');
		tmp = -tmp;
	} else {
		lcd_data('+');
	}
	lcd_data('0' + tmp / 10 % 10);
	lcd_data(',');
	lcd_data('0' + tmp % 10);
	return Calibration_edit;
}

int8_t Calibration_State_Humi(int8_t input) {
	// Wert bearbeiten?
	if (input == MENU_INPUT_PUSH) {
		Calibration_edit ^= 1;
		eeprom_write_byte(&Calibration_humidityE, Calibration_humidity);
	}
	if (Calibration_edit && (input == MENU_INPUT_DOWN)) {
		Calibration_humidity = (Calibration_humidity < 95) ? (Calibration_humidity+5) : (Calibration_humidity);
	}
	if (Calibration_edit && (input == MENU_INPUT_UP)) {
		Calibration_humidity = (Calibration_humidity > -95) ? (Calibration_humidity-5) : (Calibration_humidity);
	}
	// Ausgabe
	lcd_clear();
	lcd_home();
	if (Calibration_edit)
		lcd_data('*');
	else
		lcd_data('>');
	lcd_string_P(MENU_STR_CAL_HUMI);
	int8_t tmp = Calibration_humidity;
	if (tmp & 0x80) {
		lcd_data('-');
		tmp = -tmp;
	} else {
		lcd_data('+');
	}
	lcd_data('0' + tmp / 10 % 10);
	lcd_data(',');
	lcd_data('0' + tmp % 10);
	lcd_setcursor(0, 2);
	lcd_data(' ');
	lcd_string_P(MENU_STR_BACK);
	return Calibration_edit;
}

int8_t Calibration_State_Back(int8_t input) {
	lcd_clear();
	lcd_home();
	lcd_data(' ');
	lcd_string_P(MENU_STR_CAL_HUMI);
	int8_t tmp = Calibration_humidity;
	if (tmp & 0x80) {
		lcd_data('-');
		tmp = -tmp;
	} else {
		lcd_data('+');
	}
	lcd_data('0' + tmp / 10 % 10);
	lcd_data(',');
	lcd_data('0' + tmp % 10);
	lcd_setcursor(0, 2);
	lcd_data('>');
	lcd_string_P(MENU_STR_BACK);
	return 0;
}

void Calibration(void) {
	Calibration_humidity = eeprom_read_byte(&Calibration_humidityE);
	Calibration_temperature = eeprom_read_byte(&Calibration_temperatureE);
}