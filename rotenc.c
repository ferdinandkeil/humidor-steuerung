/************************************************************************/
/*                                                                      */
/*                      Reading rotary encoder                      */
/*                      one, two and four step encoders supported   */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                                                                      */
/************************************************************************/
#include <avr/io.h>
#include "rotenc.h"
#include <avr/interrupt.h>

volatile int8_t enc_delta;          // -128 ... 127
static int8_t last;


void encode_init( void ) {
	int8_t new;

	new = 0;
	if( PHASE_A )
		new = 3;
	if( PHASE_B )
		new ^= 1;                   // convert gray to binary
	last = new;                   // power on state
	enc_delta = 0;
}


void encode_isr( void ) {           // 1ms for manual movement
	int8_t new, diff;

	new = 0;
	if( !PHASE_A )
		new = 3;
	if( !PHASE_B )
		new ^= 1;                   // convert gray to binary
	diff = last - new;                // difference last - new
	if( diff & 1 ) {              // bit 0 = value (1)
		last = new;                 // store new as next last
		enc_delta += (diff & 2) - 1;        // bit 1 = direction (+/-)
	}
}


int8_t encode_read( void ) {       // read single step encoders
	int8_t val;

	cli();
	val = enc_delta;
	enc_delta = val & 3;
	sei();
	return val >> 2;
}