#include <util/delay.h>
#include "sensor.h"
#include "sht11.h"
#include "i2cmaster.h"
#include "task.h"
#include "calibration.h"

/* Konfiguration HYT131 Sensor */
#define SENSOR_HYT131_ADDR    0x28
#define SENSOR_HYT131_RETRIES   10

/* Zustände für HYT131 Sensor */
#define SENSOR_HYT131_READY      0
#define SENSOR_HYT131_MEAS       1
#define SENSOR_HYT131_TIMEOUT    2

/* Setzt den Retry-Zähler zurück */
#define Sensor_hyt131Success() {Sensor_hyt131Retries = SENSOR_HYT131_RETRIES;}

uint8_t Sensor_type;
int16_t Sensor_humidity;
int16_t Sensor_temperature;
uint8_t Sensor_hyt131State;
uint8_t Sensor_hyt131Retries;

static uint8_t Sensor_canRun(void* _self, uint32_t now) {
	return Sensor_isConnected() && TimedTask_canRun(_self, now);
}

static void Sensor_run(void* _self, uint32_t now) {
	static uint8_t sht1xTimeout;
	if (Sensor_type == SENSOR_TYPE_SHT1X) {
		if (!sht11_measure_finish()) {
			sht1xTimeout++;
		} else {
			sht1xTimeout = 0;
			Sensor_humidity = sht11_get_hum() + (Calibration_getHumi() * 10);
			Sensor_temperature = sht11_get_tmp() + (Calibration_getTemp() * 10);
			sht11_start_measure();
		}
		if ((sht1xTimeout > 9) || (Sensor_humidity < -32000)
			|| (Sensor_temperature < -32000)) {
			Sensor_type = SENSOR_TYPE_NONE;
			Sensor_humidity = SENSOR_ERROR_VAL;
			Sensor_temperature = SENSOR_ERROR_VAL;
			return;
		}
		Task_incRunTime(_self, 800);
	}
	if (Sensor_type == SENSOR_TYPE_HYT131) {
		/* 
		 * Quelle für die Kommunikation mit dem Sensor:
		 * http://jeelabs.org/2012/06/30/new-hyt131-sensor/
		 */
		/* Sensor bereit: Messung anfordern */
		if (Sensor_hyt131State == SENSOR_HYT131_READY) {
			if (i2c_start(SENSOR_HYT131_ADDR+I2C_WRITE)) {
				i2c_stop();
				Sensor_type = SENSOR_TYPE_NONE;
				Sensor_humidity = SENSOR_ERROR_VAL;
				Sensor_temperature = SENSOR_ERROR_VAL;
				return;
			}
			if (i2c_stop())
				goto hyt131_error;
			Sensor_hyt131State = SENSOR_HYT131_MEAS;
			Task_incRunTime(_self, 100);
		/* Messung ist erfolgt: Daten auslesen */
		} else if (Sensor_hyt131State == SENSOR_HYT131_MEAS) {
			uint16_t h, t, temp;
			if (i2c_start_wait(SENSOR_HYT131_ADDR+I2C_READ) > I2C_ERROR)
				goto hyt131_error;
			if ((temp = i2c_readAck()) > I2C_ERROR)
				goto hyt131_error;
			h = (temp & 0x3F) << 8;
			if ((temp = i2c_readAck()) > I2C_ERROR)
				goto hyt131_error;
			h |= temp;
			if ((temp = i2c_readAck()) > I2C_ERROR)
				goto hyt131_error;
			t = temp << 6;
			if ((temp = i2c_readNak()) > I2C_ERROR)
				goto hyt131_error;
			t |= temp >> 2;
			Sensor_humidity = (h * 10000L >> 14) + (Calibration_getHumi() * 10);
			Sensor_temperature = (t * 16500L >> 14) - 4000 + (Calibration_getTemp() * 10);
			Sensor_hyt131Success();   // Retry-Zähler zurücksetzen
			Sensor_hyt131State = SENSOR_HYT131_READY;
			Task_incRunTime(_self, 700);
		}
		return;

		/* Fehlerbehandlung für HYT131 Sensor */
		hyt131_error: {
			if (Sensor_hyt131Retries-- == 0) {
				Sensor_type = SENSOR_TYPE_ERROR;
				Sensor_humidity = SENSOR_ERROR_VAL;
				Sensor_temperature = SENSOR_ERROR_VAL;
				return;
			}
			Sensor_hyt131State = SENSOR_HYT131_READY;
			Task_incRunTime(_self, 1000);
			return;
		}
		
	}
	/* bei Fehler */
	Task_incRunTime(_self, 1000);
}

Task_t* Sensor(void) {
	Sensor_type = SENSOR_TYPE_SEARCHING;
	Sensor_hyt131Retries = SENSOR_HYT131_RETRIES;
	PORTC |= (1<<PC4) | (1<<PC5);
	i2c_init();
	if (!i2c_start(SENSOR_HYT131_ADDR+I2C_WRITE)) {
		i2c_stop();
		Sensor_type = SENSOR_TYPE_HYT131;
		Sensor_hyt131State = SENSOR_HYT131_MEAS;
	} else {
		TWCR = 0;
		Sensor_type = SENSOR_TYPE_SHT1X;
		sht11_init();
	}
	static Task_t sensor;
	sensor.canRun = Sensor_canRun;
	sensor.run = Sensor_run;
	sensor.runTime = 0;
	return &sensor;
}