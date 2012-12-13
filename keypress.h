/************************************************************************/
/*                                                                      */
/*                      Debouncing 8 Keys                               */
/*                      Sampling 4 Times                                */
/*                      With Repeat Function                            */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef KEYPRESS_H
#define KEYPRESS_H

#define KEY_DDR         DDRD
#define KEY_PORT        PORTD
#define KEY_PIN         PIND
#define KEY0            3
#define ALL_KEYS        (1<<KEY0)

#define REPEAT_MASK     (1<<KEY0)		       	// repeat: key1, key2
#define REPEAT_START    50                        // after 1000ms
#define REPEAT_NEXT     20                        // every 400ms

volatile uint8_t key_state;                                // debounced and inverted key state:
// bit = 1: key pressed
volatile uint8_t key_press;                                // key press detect

volatile uint8_t key_rpt;                                  // key long press and repeat

void key_press_isr(void);

///////////////////////////////////////////////////////////////////
//
// check if a key has been pressed. Each pressed key is reported
// only once
//
uint8_t get_key_press( uint8_t key_mask );

///////////////////////////////////////////////////////////////////
//
// check if a key has been pressed long enough such that the
// key repeat functionality kicks in. After a small setup delay
// the key is reported beeing pressed in subsequent calls
// to this function. This simulates the user repeatedly
// pressing and releasing the key.
//
//uint8_t get_key_rpt( uint8_t key_mask );

//uint8_t get_key_short( uint8_t key_mask );

//uint8_t get_key_long( uint8_t key_mask );

#endif
