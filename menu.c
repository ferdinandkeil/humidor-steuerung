#include "menu.h"
#include <avr/pgmspace.h>
#include "rotenc.h"
#include "keypress.h"
#include "lcd-routines.h"
#include "fans.h"
#include "clock.h"
#include "sensor.h"
#include "alarm.h"
#include "calibration.h"
#include "sht11.h"

// Variablen

uint8_t Menu_currState;

uint8_t Menu_keyPress;

int8_t Menu_rotEnc;

Task_t Menu_timeOut;

uint8_t Menu_timeOut_run;

uint8_t Menu_updateRequest;

const char Menu_cgBell[] PROGMEM = {
	0b00000, // 0
	0b00100, // 1
	0b01110, // 2
	0b01110, // 3
	0b11111, // 4
	0b11111, // 5
	0b00100, // 6
	0b00000  // 7
};

// Menu Text

const char MENU_STR_OCLOCK[] PROGMEM =         "Uhr";
const char MENU_STR_FAN1[] PROGMEM =           {'L', 0xF5, 'f', 't', 'e', 'r',' ', '1', '\0'};
const char MENU_STR_FAN2[] PROGMEM =           {'L', 0xF5, 'f', 't', 'e', 'r',' ', '2', '\0'};
const char MENU_STR_FAN3[] PROGMEM =           {'L', 0xF5, 'f', 't', 'e', 'r',' ', '3', '\0'};
const char MENU_STR_FAN4[] PROGMEM =           {'L', 0xF5, 'f', 't', 'e', 'r',' ', '4', '\0'};
const char MENU_STR_CAL[] PROGMEM =            "Kalibrierung";
const char MENU_STR_TIME[] PROGMEM =           "Uhrzeit";
const char MENU_STR_CTRL[] PROGMEM =           "Steuerng:  ";
const char MENU_STR_CTRL_OFF[] PROGMEM =       " aus";
const char MENU_STR_CTRL_HUMI[] PROGMEM =      "relF";
const char MENU_STR_CTRL_TIME[] PROGMEM =      "Zeit";
const char MENU_STR_CTRL_REP[] PROGMEM =       "Intv";
const char MENU_STR_HUMIDITY[] PROGMEM =       "Grenzwert:  ";
const char MENU_STR_ONTIME[] PROGMEM =         "Zeit:     ";
const char MENU_STR_REPEAT[] PROGMEM =         "Intervall:  ";
const char MENU_STR_DURATION[] PROGMEM =       "Dauer:    ";
const char MENU_STR_MIN[] PROGMEM =            "min";
const char MENU_STR_BACK[] PROGMEM =           {'(', 'Z', 'u', 'r', 0xF5, 'c', 'k', ')', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'};
const char MENU_STR_CAL_TEMP[] PROGMEM =       "Temp.:     ";
const char MENU_STR_CAL_HUMI[] PROGMEM =       "Rel.F.:    ";
const char MENU_STR_HOUR[] PROGMEM =           "Stunden:     ";
const char MENU_STR_MINUTES[] PROGMEM =        "Minuten:     ";
const char MENU_STR_ALARM[] PROGMEM =          "Alarm";
const char MENU_STR_ALARM_ENABLE[] PROGMEM =   "Alarm:      ";
const char MENU_STR_ON[] PROGMEM =             " an";
const char MENU_STR_OFF[] PROGMEM =            "aus";
const char MENU_STR_LED[] PROGMEM =            "LED:        ";
const char MENU_STR_BUZZER[] PROGMEM =         "Piepser:    ";
const char MENU_STR_ALARM_STATUS[] PROGMEM =   " Alarm!";
const char MENU_STR_NO_SENSOR[] PROGMEM =      "Kein Sensor!";
const char MENU_STR_SEARCHING[] PROGMEM =      "Suche Sensor...";
const char MENU_STR_SENSOR_ERROR[] PROGMEM =   "Sensor Fehler!";

const Menu_State_t Menu_state[] PROGMEM = {
	{MENU_STATE_STATUS,  Menu_Root},
	{MENU_STATE_FAN1  ,  Menu_Root},
	{MENU_STATE_FAN2  ,  Menu_Root},
	{MENU_STATE_FAN3  ,  Menu_Root},
	{MENU_STATE_FAN4  ,  Menu_Root},
	{MENU_STATE_ALARM ,  Menu_Root},
	{MENU_STATE_CAL   ,  Menu_Root},
	{MENU_STATE_TIME  ,  Menu_Root},
	{MENU_STATE_BACK  ,  Menu_Root},
	
	{MENU_STATE_FAN1+MENU_STATE_FAN_CTRL, Fan_State_1_Ctrl},
	{MENU_STATE_FAN1+MENU_STATE_FAN_HUMI, Fan_State_1_Humi},
	{MENU_STATE_FAN1+MENU_STATE_FAN_ON  , Fan_State_1_On  },
	{MENU_STATE_FAN1+MENU_STATE_FAN_REP , Fan_State_1_Rep },
	{MENU_STATE_FAN1+MENU_STATE_FAN_DUR , Fan_State_1_Dur },
	{MENU_STATE_FAN1+MENU_STATE_FAN_BACK, Fan_State_1_Back},

	{MENU_STATE_FAN2+MENU_STATE_FAN_CTRL, Fan_State_2_Ctrl},
	{MENU_STATE_FAN2+MENU_STATE_FAN_HUMI, Fan_State_2_Humi},
	{MENU_STATE_FAN2+MENU_STATE_FAN_ON  , Fan_State_2_On  },
	{MENU_STATE_FAN2+MENU_STATE_FAN_REP , Fan_State_2_Rep },
	{MENU_STATE_FAN2+MENU_STATE_FAN_DUR , Fan_State_2_Dur },
	{MENU_STATE_FAN2+MENU_STATE_FAN_BACK, Fan_State_2_Back},
	
	{MENU_STATE_FAN3+MENU_STATE_FAN_CTRL, Fan_State_3_Ctrl},
	{MENU_STATE_FAN3+MENU_STATE_FAN_HUMI, Fan_State_3_Humi},
	{MENU_STATE_FAN3+MENU_STATE_FAN_ON  , Fan_State_3_On  },
	{MENU_STATE_FAN3+MENU_STATE_FAN_REP , Fan_State_3_Rep },
	{MENU_STATE_FAN3+MENU_STATE_FAN_DUR , Fan_State_3_Dur },
	{MENU_STATE_FAN3+MENU_STATE_FAN_BACK, Fan_State_3_Back},
	
	{MENU_STATE_FAN4+MENU_STATE_FAN_CTRL, Fan_State_4_Ctrl},
	{MENU_STATE_FAN4+MENU_STATE_FAN_HUMI, Fan_State_4_Humi},
	{MENU_STATE_FAN4+MENU_STATE_FAN_ON  , Fan_State_4_On  },
	{MENU_STATE_FAN4+MENU_STATE_FAN_REP , Fan_State_4_Rep },
	{MENU_STATE_FAN4+MENU_STATE_FAN_DUR , Fan_State_4_Dur },
	{MENU_STATE_FAN4+MENU_STATE_FAN_BACK, Fan_State_4_Back},
	
	{MENU_STATE_ALARM_EN  , Alarm_State_Enable  },
	{MENU_STATE_ALARM_HUMI, Alarm_State_Humidity},
	{MENU_STATE_ALARM_LED , Alarm_State_Led     },
	{MENU_STATE_ALARM_BUZ , Alarm_State_Buzzer  },
	{MENU_STATE_ALARM_BACK, Alarm_State_Back    },
	
	{MENU_STATE_TIME_H   , Clock_State_Hour  },
	{MENU_STATE_TIME_M   , Clock_State_Minute},
	{MENU_STATE_TIME_BACK, Clock_State_Back  },
	
	{MENU_STATE_CAL_TEMP, Calibration_State_Temp},
	{MENU_STATE_CAL_HUMI, Calibration_State_Humi},
	{MENU_STATE_CAL_BACK, Calibration_State_Back},
	
	{0                ,  0        }
};

const Menu_NextState_t Menu_nextState[] PROGMEM = {
	// Hauptmenü

	{MENU_STATE_STATUS,  MENU_INPUT_PUSH,  MENU_STATE_FAN1    },
	
	{MENU_STATE_FAN1  ,  MENU_INPUT_PUSH,  MENU_STATE_FAN1+MENU_STATE_FAN_CTRL},
	{MENU_STATE_FAN1  ,  MENU_INPUT_DOWN,  MENU_STATE_FAN2    },
	
	{MENU_STATE_FAN2  ,  MENU_INPUT_UP  ,  MENU_STATE_FAN1    },
	{MENU_STATE_FAN2  ,  MENU_INPUT_PUSH,  MENU_STATE_FAN2+MENU_STATE_FAN_CTRL},
	{MENU_STATE_FAN2  ,  MENU_INPUT_DOWN,  MENU_STATE_FAN3    },
		
	{MENU_STATE_FAN3  ,  MENU_INPUT_UP  ,  MENU_STATE_FAN2    },
	{MENU_STATE_FAN3  ,  MENU_INPUT_PUSH,  MENU_STATE_FAN3+MENU_STATE_FAN_CTRL},
	{MENU_STATE_FAN3  ,  MENU_INPUT_DOWN,  MENU_STATE_FAN4    },
	
	{MENU_STATE_FAN4  ,  MENU_INPUT_UP  ,  MENU_STATE_FAN3    },
	{MENU_STATE_FAN4  ,  MENU_INPUT_PUSH,  MENU_STATE_FAN4+MENU_STATE_FAN_CTRL},
	{MENU_STATE_FAN4  ,  MENU_INPUT_DOWN,  MENU_STATE_ALARM   },
	
	{MENU_STATE_ALARM ,  MENU_INPUT_UP  ,  MENU_STATE_FAN4    },
	{MENU_STATE_ALARM ,  MENU_INPUT_PUSH,  MENU_STATE_ALARM_EN},
	{MENU_STATE_ALARM ,  MENU_INPUT_DOWN,  MENU_STATE_CAL     },
	
	{MENU_STATE_CAL   ,  MENU_INPUT_UP  ,  MENU_STATE_ALARM   },
	{MENU_STATE_CAL   ,  MENU_INPUT_PUSH,  MENU_STATE_CAL_TEMP},
	{MENU_STATE_CAL   ,  MENU_INPUT_DOWN,  MENU_STATE_TIME    },
	
	{MENU_STATE_TIME  ,  MENU_INPUT_UP  ,  MENU_STATE_CAL     },
	{MENU_STATE_TIME  ,  MENU_INPUT_PUSH,  MENU_STATE_TIME_H  },
	{MENU_STATE_TIME  ,  MENU_INPUT_DOWN,  MENU_STATE_BACK    },
	
	{MENU_STATE_BACK  ,  MENU_INPUT_UP  ,  MENU_STATE_TIME    },
	{MENU_STATE_BACK  ,  MENU_INPUT_PUSH,  MENU_STATE_STATUS  },
	
	// Menü Lüfter 1
	
	{MENU_STATE_FAN1+MENU_STATE_FAN_CTRL  ,  MENU_INPUT_DOWN,  MENU_STATE_FAN1+MENU_STATE_FAN_HUMI},
	
	{MENU_STATE_FAN1+MENU_STATE_FAN_HUMI  ,  MENU_INPUT_UP  ,  MENU_STATE_FAN1+MENU_STATE_FAN_CTRL},
	{MENU_STATE_FAN1+MENU_STATE_FAN_HUMI  ,  MENU_INPUT_DOWN,  MENU_STATE_FAN1+MENU_STATE_FAN_ON  },
	
	{MENU_STATE_FAN1+MENU_STATE_FAN_ON    ,  MENU_INPUT_UP  ,  MENU_STATE_FAN1+MENU_STATE_FAN_HUMI},
	{MENU_STATE_FAN1+MENU_STATE_FAN_ON    ,  MENU_INPUT_DOWN,  MENU_STATE_FAN1+MENU_STATE_FAN_REP },
	
	{MENU_STATE_FAN1+MENU_STATE_FAN_REP   ,  MENU_INPUT_UP  ,  MENU_STATE_FAN1+MENU_STATE_FAN_ON  },
	{MENU_STATE_FAN1+MENU_STATE_FAN_REP   ,  MENU_INPUT_DOWN,  MENU_STATE_FAN1+MENU_STATE_FAN_DUR },
	
	{MENU_STATE_FAN1+MENU_STATE_FAN_DUR   ,  MENU_INPUT_UP  ,  MENU_STATE_FAN1+MENU_STATE_FAN_REP },
	{MENU_STATE_FAN1+MENU_STATE_FAN_DUR   ,  MENU_INPUT_DOWN,  MENU_STATE_FAN1+MENU_STATE_FAN_BACK},
	
	{MENU_STATE_FAN1+MENU_STATE_FAN_BACK  ,  MENU_INPUT_UP  ,  MENU_STATE_FAN1+MENU_STATE_FAN_DUR },
	{MENU_STATE_FAN1+MENU_STATE_FAN_BACK  ,  MENU_INPUT_PUSH,  MENU_STATE_FAN1},
	
	// Menü Lüfter 2
	
	{MENU_STATE_FAN2+MENU_STATE_FAN_CTRL  ,  MENU_INPUT_DOWN,  MENU_STATE_FAN2+MENU_STATE_FAN_HUMI},
	
	{MENU_STATE_FAN2+MENU_STATE_FAN_HUMI  ,  MENU_INPUT_UP  ,  MENU_STATE_FAN2+MENU_STATE_FAN_CTRL},
	{MENU_STATE_FAN2+MENU_STATE_FAN_HUMI  ,  MENU_INPUT_DOWN,  MENU_STATE_FAN2+MENU_STATE_FAN_ON  },
	
	{MENU_STATE_FAN2+MENU_STATE_FAN_ON    ,  MENU_INPUT_UP  ,  MENU_STATE_FAN2+MENU_STATE_FAN_HUMI},
	{MENU_STATE_FAN2+MENU_STATE_FAN_ON    ,  MENU_INPUT_DOWN,  MENU_STATE_FAN2+MENU_STATE_FAN_REP },
	
	{MENU_STATE_FAN2+MENU_STATE_FAN_REP   ,  MENU_INPUT_UP  ,  MENU_STATE_FAN2+MENU_STATE_FAN_ON  },
	{MENU_STATE_FAN2+MENU_STATE_FAN_REP   ,  MENU_INPUT_DOWN,  MENU_STATE_FAN2+MENU_STATE_FAN_DUR },
	
	{MENU_STATE_FAN2+MENU_STATE_FAN_DUR   ,  MENU_INPUT_UP  ,  MENU_STATE_FAN2+MENU_STATE_FAN_REP },
	{MENU_STATE_FAN2+MENU_STATE_FAN_DUR   ,  MENU_INPUT_DOWN,  MENU_STATE_FAN2+MENU_STATE_FAN_BACK},
	
	{MENU_STATE_FAN2+MENU_STATE_FAN_BACK  ,  MENU_INPUT_UP  ,  MENU_STATE_FAN2+MENU_STATE_FAN_DUR },
	{MENU_STATE_FAN2+MENU_STATE_FAN_BACK  ,  MENU_INPUT_PUSH,  MENU_STATE_FAN2},
	
	// Menü Lüfter 3
	
	{MENU_STATE_FAN3+MENU_STATE_FAN_CTRL  ,  MENU_INPUT_DOWN,  MENU_STATE_FAN3+MENU_STATE_FAN_HUMI},
	
	{MENU_STATE_FAN3+MENU_STATE_FAN_HUMI  ,  MENU_INPUT_UP  ,  MENU_STATE_FAN3+MENU_STATE_FAN_CTRL},
	{MENU_STATE_FAN3+MENU_STATE_FAN_HUMI  ,  MENU_INPUT_DOWN,  MENU_STATE_FAN3+MENU_STATE_FAN_ON  },
	
	{MENU_STATE_FAN3+MENU_STATE_FAN_ON    ,  MENU_INPUT_UP  ,  MENU_STATE_FAN3+MENU_STATE_FAN_HUMI},
	{MENU_STATE_FAN3+MENU_STATE_FAN_ON    ,  MENU_INPUT_DOWN,  MENU_STATE_FAN3+MENU_STATE_FAN_REP },
	
	{MENU_STATE_FAN3+MENU_STATE_FAN_REP   ,  MENU_INPUT_UP  ,  MENU_STATE_FAN3+MENU_STATE_FAN_ON  },
	{MENU_STATE_FAN3+MENU_STATE_FAN_REP   ,  MENU_INPUT_DOWN,  MENU_STATE_FAN3+MENU_STATE_FAN_DUR },
	
	{MENU_STATE_FAN3+MENU_STATE_FAN_DUR   ,  MENU_INPUT_UP  ,  MENU_STATE_FAN3+MENU_STATE_FAN_REP },
	{MENU_STATE_FAN3+MENU_STATE_FAN_DUR   ,  MENU_INPUT_DOWN,  MENU_STATE_FAN3+MENU_STATE_FAN_BACK},
	
	{MENU_STATE_FAN3+MENU_STATE_FAN_BACK  ,  MENU_INPUT_UP  ,  MENU_STATE_FAN3+MENU_STATE_FAN_DUR },
	{MENU_STATE_FAN3+MENU_STATE_FAN_BACK  ,  MENU_INPUT_PUSH,  MENU_STATE_FAN3},
	
	// Menü Lüfter 4
	
	{MENU_STATE_FAN4+MENU_STATE_FAN_CTRL  ,  MENU_INPUT_DOWN,  MENU_STATE_FAN4+MENU_STATE_FAN_HUMI},
	
	{MENU_STATE_FAN4+MENU_STATE_FAN_HUMI  ,  MENU_INPUT_UP  ,  MENU_STATE_FAN4+MENU_STATE_FAN_CTRL},
	{MENU_STATE_FAN4+MENU_STATE_FAN_HUMI  ,  MENU_INPUT_DOWN,  MENU_STATE_FAN4+MENU_STATE_FAN_ON  },
	
	{MENU_STATE_FAN4+MENU_STATE_FAN_ON    ,  MENU_INPUT_UP  ,  MENU_STATE_FAN4+MENU_STATE_FAN_HUMI},
	{MENU_STATE_FAN4+MENU_STATE_FAN_ON    ,  MENU_INPUT_DOWN,  MENU_STATE_FAN4+MENU_STATE_FAN_REP },
	
	{MENU_STATE_FAN4+MENU_STATE_FAN_REP   ,  MENU_INPUT_UP  ,  MENU_STATE_FAN4+MENU_STATE_FAN_ON  },
	{MENU_STATE_FAN4+MENU_STATE_FAN_REP   ,  MENU_INPUT_DOWN,  MENU_STATE_FAN4+MENU_STATE_FAN_DUR },
	
	{MENU_STATE_FAN4+MENU_STATE_FAN_DUR   ,  MENU_INPUT_UP  ,  MENU_STATE_FAN4+MENU_STATE_FAN_REP },
	{MENU_STATE_FAN4+MENU_STATE_FAN_DUR   ,  MENU_INPUT_DOWN,  MENU_STATE_FAN4+MENU_STATE_FAN_BACK},
	
	{MENU_STATE_FAN4+MENU_STATE_FAN_BACK  ,  MENU_INPUT_UP  ,  MENU_STATE_FAN4+MENU_STATE_FAN_DUR },
	{MENU_STATE_FAN4+MENU_STATE_FAN_BACK  ,  MENU_INPUT_PUSH,  MENU_STATE_FAN4},
	
	// Menü Alarm
	
	{MENU_STATE_ALARM_EN  ,  MENU_INPUT_DOWN,  MENU_STATE_ALARM_HUMI},
	
	{MENU_STATE_ALARM_HUMI,  MENU_INPUT_UP  ,  MENU_STATE_ALARM_EN  },
	{MENU_STATE_ALARM_HUMI,  MENU_INPUT_DOWN,  MENU_STATE_ALARM_LED },
	
	{MENU_STATE_ALARM_LED ,  MENU_INPUT_UP  ,  MENU_STATE_ALARM_HUMI},
	{MENU_STATE_ALARM_LED ,  MENU_INPUT_DOWN,  MENU_STATE_ALARM_BUZ },
	
	{MENU_STATE_ALARM_BUZ ,  MENU_INPUT_UP  ,  MENU_STATE_ALARM_LED },
	{MENU_STATE_ALARM_BUZ ,  MENU_INPUT_DOWN,  MENU_STATE_ALARM_BACK},
	
	{MENU_STATE_ALARM_BACK,  MENU_INPUT_UP  ,  MENU_STATE_ALARM_BUZ },
	{MENU_STATE_ALARM_BACK,  MENU_INPUT_PUSH,  MENU_STATE_ALARM     },
	
	// Menü Uhrzeit
	
	{MENU_STATE_TIME_H   ,  MENU_INPUT_DOWN,  MENU_STATE_TIME_M   },
	
	{MENU_STATE_TIME_M   ,  MENU_INPUT_UP  ,  MENU_STATE_TIME_H   },
	{MENU_STATE_TIME_M   ,  MENU_INPUT_DOWN,  MENU_STATE_TIME_BACK},
	
	{MENU_STATE_TIME_BACK,  MENU_INPUT_UP  ,  MENU_STATE_TIME_M   },
	{MENU_STATE_TIME_BACK,  MENU_INPUT_PUSH,  MENU_STATE_TIME     },
	
	// Menü Kalibrierung
	
	{MENU_STATE_CAL_TEMP,  MENU_INPUT_DOWN,  MENU_STATE_CAL_HUMI},
	
	{MENU_STATE_CAL_HUMI,  MENU_INPUT_UP  ,  MENU_STATE_CAL_TEMP},
	{MENU_STATE_CAL_HUMI,  MENU_INPUT_DOWN,  MENU_STATE_CAL_BACK},
	
	{MENU_STATE_CAL_BACK,  MENU_INPUT_UP  ,  MENU_STATE_CAL_HUMI},
	{MENU_STATE_CAL_BACK,  MENU_INPUT_PUSH,  MENU_STATE_CAL     },
	
	// NULL-Bytes
	
	{0                ,  0              ,  0                  }
};

int8_t Menu_Root(int8_t input) {
	lcd_clear();
	lcd_home();	
	switch (Menu_currState) {
		case MENU_STATE_STATUS:
			lcd_number(Clock_getHour(), 2, '0');
			lcd_data( 0x3A ); // :
			lcd_number(Clock_getMinute(), 2, '0');
			lcd_data( 0x3A ); // :
			lcd_number(Clock_getSecond(), 2, '0');
			lcd_data(' ');
			lcd_string_P(MENU_STR_OCLOCK);
			lcd_setcursor(0, 2);
			static uint8_t blink;
			if (Alarm_wasTriggered() && (blink++ & 4)) {
				lcd_data(LCD_GC_CHAR0);
				lcd_string_P(MENU_STR_ALARM_STATUS);
			} else {
				switch (Sensor_getType()) {
					case SENSOR_TYPE_SEARCHING:
						lcd_string_P(MENU_STR_SEARCHING);
						break;
					default:
					case SENSOR_TYPE_NONE:
						lcd_string_P(MENU_STR_NO_SENSOR);
						break;
					case SENSOR_TYPE_ERROR:
						lcd_string_P(MENU_STR_SENSOR_ERROR);
						break;
					case SENSOR_TYPE_SHT1X:
					case SENSOR_TYPE_HYT131:
						lcd_number(Sensor_getTemp() / 100, 2, '0');
						lcd_data(',');
						lcd_number(Sensor_getTemp() % 100, 2, '0');
						lcd_data( 0xDF ); // °
						lcd_data('C');
						lcd_data(' ');
						lcd_data(' ');
						lcd_data(' ');
						lcd_number(Sensor_getHumi() / 100, 2, '0');
						lcd_data(',');
						lcd_number(Sensor_getHumi() % 100, 2, '0');
						lcd_data('%');
						break;	
				}
			}
			break;
		case MENU_STATE_FAN1:
			lcd_data('>');
			lcd_string_P(MENU_STR_FAN1);
			lcd_setcursor(0, 2);
			lcd_data(' ');
			lcd_string_P(MENU_STR_FAN2);
			break;
		case MENU_STATE_FAN2:
			lcd_data('>');
			lcd_string_P(MENU_STR_FAN2);
			lcd_setcursor(0, 2);
			lcd_data(' ');
			lcd_string_P(MENU_STR_FAN3);
			break;
		case MENU_STATE_FAN3:
			lcd_data('>');
			lcd_string_P(MENU_STR_FAN3);
			lcd_setcursor(0, 2);
			lcd_data(' ');
			lcd_string_P(MENU_STR_FAN4);
			break;
		case MENU_STATE_FAN4:
			lcd_data('>');
			lcd_string_P(MENU_STR_FAN4);
			lcd_setcursor(0, 2);
			lcd_data(' ');
			lcd_string_P(MENU_STR_ALARM);
			break;
		case MENU_STATE_ALARM:
			lcd_data('>');
			lcd_string_P(MENU_STR_ALARM);
			lcd_setcursor(0, 2);
			lcd_data(' ');
			lcd_string_P(MENU_STR_CAL);
			break;
		case MENU_STATE_CAL:
			lcd_data('>');
			lcd_string_P(MENU_STR_CAL);
			lcd_setcursor(0, 2);
			lcd_data(' ');
			lcd_string_P(MENU_STR_TIME);
			break;
		case MENU_STATE_TIME:
			lcd_data('>');
			lcd_string_P(MENU_STR_TIME);
			lcd_setcursor(0, 2);
			lcd_data(' ');
			lcd_string_P(MENU_STR_BACK);
			break;
		case MENU_STATE_BACK:
			lcd_data(' ');
			lcd_string_P(MENU_STR_TIME);
			lcd_setcursor(0, 2);
			lcd_data('>');
			lcd_string_P(MENU_STR_BACK);
			break;
	}
	return 0;
}

static inline uint8_t Menu_statemachine(uint8_t state, uint8_t stimuli) {
    uint8_t nextState = state;    // Default stay in same state
    uint8_t i, j;

	for (i=0; ( j=pgm_read_byte(&Menu_nextState[i].state) ); i++ ) {
		if ( j == state && 
		pgm_read_byte(&Menu_nextState[i].input) == stimuli) {
			// This is the one!
			nextState = pgm_read_byte(&Menu_nextState[i].nextstate);
			break;
		}
	}

    return nextState;
}

static inline uint8_t Menu_stateRun(uint8_t state, uint8_t stimuli) {
	uint8_t i, j;
	
	for (i = 0; ( j=pgm_read_byte(&Menu_state[i].state) ); i++) {
		if (j == state) {
			// Funktionspointer, Typ in Header definiert
			Menu_stateFunc_t func;
			// Beachte: Pointer sind 2 Byte lang
			func = (PGM_VOID_P) pgm_read_word(&Menu_state[i].func);
			// Funktionsaufruf
			return func(stimuli);
		}
	}
	return 0;
}

static uint8_t Menu_canRun(void *_self, uint32_t now) {
	Menu_keyPress = get_key_press(1<<KEY0);
	Menu_rotEnc = encode_read();
	return Menu_keyPress || Menu_rotEnc || Menu_updateRequest;
}

static void Menu_run(void *_self, uint32_t now) {
	// Stimulus durch Update
	uint8_t stimuli = Menu_updateRequest;
	// Eingabemöglichkeiten, geordnet nach Priorität
	if (Menu_keyPress) {
		stimuli = MENU_INPUT_PUSH;
	} else if (Menu_rotEnc) {
		if (Menu_rotEnc & 128)
		#if MENU_ENC_TYPE == MENU_ENC_TYPE_NOBLE
			stimuli = MENU_INPUT_UP;
		else
			stimuli = MENU_INPUT_DOWN;
		#else
			stimuli = MENU_INPUT_DOWN;
		else
			stimuli = MENU_INPUT_UP;
		#endif
	}
	// Wenn Funktion für Zustand 1 zurück gibt, wird der Zustand blockiert
	static uint8_t lockState;
	if (!lockState) {
		uint8_t lastState = Menu_currState;
		Menu_currState = Menu_statemachine(Menu_currState, stimuli);
		if (Menu_currState != lastState) {
			// Eingabe schon "verbraucht", daher zurücksetzen
			stimuli = 0;
		}
	} else {
		// Kein TimeOut bei blockiertem Zustand
		Menu_timeOut_run = 0;
	}
	lockState = Menu_stateRun(Menu_currState, stimuli);
	// Wenn nicht in Status-Anzeige, TimeOut setzen
	if ((Menu_currState != MENU_STATE_STATUS) && (stimuli != MENU_INPUT_CLOCK) && (!lockState)) {
		Task_setRunTime(&Menu_timeOut, millis()+MENU_TIMEOUT_MS);
		Menu_timeOut_run = 1;
	}
	// Eingaben zurücksetzen
	Menu_updateRequest = 0;
	Menu_rotEnc = 0;
	Menu_keyPress = 0;
}

Task_t* Menu(void) {
	Menu_currState = MENU_STATE_STATUS;
	Menu_keyPress = 0;
	Menu_rotEnc = 0;
	Menu_updateRequest = 0;
	lcd_init();
	lcd_generatechar_P(LCD_GC_CHAR0, Menu_cgBell, 8);
	static Task_t menu;
	menu.canRun = Menu_canRun;
	menu.run = Menu_run;
	menu.runTime = 0;
	Menu_Root(0);
	return &menu;
}

static uint8_t Menu_TimeOut_canRun(void *_self, uint32_t now) {
	return TimedTask_canRun(_self, now) && Menu_timeOut_run;
}

static void Menu_TimeOut_run(void *_self, uint32_t now) {
	Menu_currState = MENU_STATE_STATUS;
	Menu_update(MENU_INPUT_TIMEOUT);
	Menu_timeOut_run = 0;
}

Task_t* Menu_TimeOut(void) {
	Menu_timeOut_run = 0;
	Menu_timeOut.canRun = Menu_TimeOut_canRun;
	Menu_timeOut.run = Menu_TimeOut_run;
	Menu_timeOut.runTime = 0;
	return &Menu_timeOut;
}