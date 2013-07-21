#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>
#include "task.h"

typedef struct {
	uint8_t status;
	uint8_t humidity;
} Alarm_t;

extern uint8_t Alarm_triggered;

#define Alarm_wasTriggered() Alarm_triggered

int8_t Alarm_State_Enable(int8_t input);

int8_t Alarm_State_Humidity(int8_t input);

int8_t Alarm_State_Led(int8_t input);

int8_t Alarm_State_Buzzer(int8_t input);

int8_t Alarm_State_Back(int8_t input);

Task_t* Led(void);

Task_t* Alarm(void);

#endif