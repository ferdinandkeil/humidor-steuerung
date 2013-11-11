#ifndef MENU_H
#define MENU_H

#include <avr/pgmspace.h>
#include "task.h"

// Config

#define MENU_ENC_TYPE_NOBLE    0
#define MENU_ENC_TYPE_NONAME   1

#define MENU_ENC_TYPE          MENU_ENC_TYPE_NONAME

// Menu Konstanten

#define MENU_NULL              0

#define MENU_STATE_FAN_CTRL	   1
#define MENU_STATE_FAN_HUMI	   2
#define MENU_STATE_FAN_ON	   3
#define MENU_STATE_FAN_REP	   4
#define MENU_STATE_FAN_DUR	   5
#define MENU_STATE_FAN_BACK	   9

#define MENU_STATE_STATUS	   1
#define MENU_STATE_FAN1       10
#define MENU_STATE_FAN2       20
#define MENU_STATE_FAN3	      30
#define MENU_STATE_FAN4	      40
#define MENU_STATE_RESERVED   50
#define MENU_STATE_CAL        60
#define MENU_STATE_CAL_TEMP   61
#define MENU_STATE_CAL_HUMI   62
#define MENU_STATE_CAL_BACK   69
#define MENU_STATE_TIME       70
#define MENU_STATE_TIME_H     71
#define MENU_STATE_TIME_M     72
#define MENU_STATE_TIME_BACK  79
#define MENU_STATE_ALARM      80
#define MENU_STATE_ALARM_EN   81
#define MENU_STATE_ALARM_HUMI 82
#define MENU_STATE_ALARM_LED  83
#define MENU_STATE_ALARM_BUZ  84
#define MENU_STATE_ALARM_BACK 89
#define MENU_STATE_VER        90
#define MENU_STATE_VER_SHOW   91
#define MENU_STATE_BACK      100

#define MENU_INPUT_NULL        0
#define MENU_INPUT_UP          1
#define MENU_INPUT_DOWN        2
#define MENU_INPUT_PUSH        3
#define MENU_INPUT_CLOCK      10
#define MENU_INPUT_TIMEOUT    11

#define MENU_TIMEOUT_MS    10000

// Menu Text

extern const char MENU_STR_OCLOCK[] PROGMEM;
extern const char MENU_STR_FAN1[] PROGMEM;
extern const char MENU_STR_FAN2[] PROGMEM;
extern const char MENU_STR_FAN3[] PROGMEM;
extern const char MENU_STR_FAN4[] PROGMEM;
extern const char MENU_STR_CAL[] PROGMEM;
extern const char MENU_STR_TIME[] PROGMEM;
extern const char MENU_STR_BOOT[] PROGMEM;
extern const char MENU_STR_CTRL[] PROGMEM;
extern const char MENU_STR_CTRL_OFF[] PROGMEM;
extern const char MENU_STR_CTRL_HUMI[] PROGMEM;
extern const char MENU_STR_CTRL_TIME[] PROGMEM;
extern const char MENU_STR_CTRL_REP[] PROGMEM;
extern const char MENU_STR_HUMIDITY[] PROGMEM;
extern const char MENU_STR_ONTIME[] PROGMEM;
extern const char MENU_STR_REPEAT[] PROGMEM;
extern const char MENU_STR_DURATION[] PROGMEM;
extern const char MENU_STR_MIN[] PROGMEM;
extern const char MENU_STR_BACK[] PROGMEM;
extern const char MENU_STR_CAL_TEMP[] PROGMEM;
extern const char MENU_STR_CAL_HUMI[] PROGMEM;
extern const char MENU_STR_HOUR[] PROGMEM;
extern const char MENU_STR_MINUTES[] PROGMEM;
extern const char MENU_STR_ALARM[] PROGMEM;
extern const char MENU_STR_ALARM_ENABLE[] PROGMEM;
extern const char MENU_STR_ON[] PROGMEM;
extern const char MENU_STR_OFF[] PROGMEM;
extern const char MENU_STR_LED[] PROGMEM;
extern const char MENU_STR_BUZZER[] PROGMEM;
extern const char MENU_STR_ALARM_STATUS[] PROGMEM;
extern const char MENU_STR_NO_SENSOR[] PROGMEM;
extern const char MENU_STR_SEARCHING[] PROGMEM;

// Typedefs

typedef int8_t (*Menu_stateFunc_t)(int8_t input);

typedef struct PROGMEM {
    uint8_t state;
    Menu_stateFunc_t func;
} Menu_State_t;

typedef struct PROGMEM {
    uint8_t state;
    uint8_t input;
    uint8_t nextstate;
} Menu_NextState_t;

// Variablen

extern uint8_t Menu_updateRequest;

extern const char GITVERSION[];

#define Menu_update(input) Menu_updateRequest = input

// Funktionen

Task_t* Menu(void);

Task_t* Menu_TimeOut(void);

#endif
