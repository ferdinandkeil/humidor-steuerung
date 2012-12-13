/************************************************************************/
/*                                                                      */
/*                      Reading rotary encoder                    	    */
/*                      one, two and four step encoders supported       */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                                                                      */
/************************************************************************/

#include <stdint.h>

#ifndef ROTENC_H
#define ROTENC_H

#define PHASE_A     (PIND & 1<<PD4)
#define PHASE_B     (PIND & 1<<PD5)

// Initialisierung: muss am Anfang des Programms stehen
void encode_init( void );

// Interrupt Routine: muss regelmäßig aufgerufen werden
void encode_isr( void );

// Drehgeber auslesen
int8_t encode_read( void );

#endif