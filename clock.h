#ifndef CLOCK_H
#define CLOCK_H

#include "task.h"

extern uint8_t Clock_s;
extern uint8_t Clock_m;
extern uint8_t Clock_h;

int8_t Clock_State_Hour(int8_t input);
int8_t Clock_State_Minute(int8_t input);
int8_t Clock_State_Back(int8_t input);

#define Clock_getSecond() Clock_s
#define Clock_getMinute() Clock_m
#define Clock_getHour() Clock_h

Task_t* Clock(void);

#endif