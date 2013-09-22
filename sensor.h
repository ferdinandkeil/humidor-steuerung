#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include "task.h"

#define SENSOR_TYPE_SEARCHING  0
#define SENSOR_TYPE_NONE       1
#define SENSOR_TYPE_SHT1X      2
#define SENSOR_TYPE_HYT131     3
#define SENSOR_TYPE_ERROR      4

#define SENSOR_ERROR_VAL       INT16_MAX

extern uint8_t Sensor_type;
extern int16_t Sensor_humidity;
extern int16_t Sensor_temperature;

#define Sensor_isConnected() (Sensor_type > SENSOR_TYPE_NONE)

#define Sensor_isType(type) (Sensor_type == type)
#define Sensor_getType() Sensor_type

#define Sensor_getHumi() Sensor_humidity
#define Sensor_getTemp() Sensor_temperature

Task_t* Sensor(void);

#endif