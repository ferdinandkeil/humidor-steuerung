#include <util/delay.h>
#include "sensor.h"
#include "sht11.h"
#include "task.h"
#include "calibration.h"


uint8_t Sensor_type;
int16_t Sensor_humidity;
int16_t Sensor_temperature;
uint8_t Sensor_hyt131State;

static uint8_t Sensor_canRun(void* _self, uint32_t now) {
	return Sensor_isConnected() && TimedTask_canRun(_self, now);
}

static void Sensor_run(void* _self, uint32_t now) {
	static uint8_t sht1xTimeout;
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

Task_t* Sensor(void) {
	Sensor_type = SENSOR_TYPE_SHT1X;
	sht11_init();
	static Task_t sensor;
	sensor.canRun = Sensor_canRun;
	sensor.run = Sensor_run;
	sensor.runTime = 0;
	return &sensor;
}