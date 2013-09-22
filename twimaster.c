/*************************************************************************
* Title:    I2C master library using hardware TWI interface
* Author:   Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury
* File:     $Id: twimaster.c,v 1.3 2005/07/02 11:14:21 Peter Exp $
* Software: AVR-GCC 3.4.3 / avr-libc 1.2.3
* Target:   any AVR device with hardware TWI 
* Usage:    API compatible with I2C Software Library i2cmaster.h
**************************************************************************/
#include <inttypes.h>
#include <compat/twi.h>

#include <i2cmaster.h>


/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 12000000UL
#endif

/* I2C clock in Hz */
#define SCL_CLOCK  100000L

/* I2C timeout */
#define I2C_TIMEOUT 2000UL

/* wait with timeout */
#define I2C_wait_while(condition) {\
	                            	uint16_t timeout = I2C_TIMEOUT; \
	                            	while (condition) { \
	                            		if (timeout-- == 0) { \
	                            			TWCR = 0; \
	                            			return I2C_ERROR_TIMEOUT; \
	                            		} \
	                            	} \
                            	}


/*************************************************************************
 Initialization of the I2C bus interface. Need to be called only once
*************************************************************************/
void i2c_init(void)
{
  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
  
  TWSR = 0;                         /* no prescaler */
  TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  /* must be > 10 for stable operation */

}/* i2c_init */


/*************************************************************************	
  Issues a start condition and sends address and transfer direction.
  return I2C_SUCCESS = device accessible, I2C_ERROR_*= failed to access device
*************************************************************************/
uint16_t i2c_start(unsigned char address)
{
    uint8_t   twst;

	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	I2C_wait_while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_START) && (twst != TW_REP_START)) return I2C_ERROR_START;

	// send device address
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	I2C_wait_while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return I2C_ERROR_WRITE;

	return I2C_SUCCESS;

}/* i2c_start */


/*************************************************************************
 Issues a start condition and sends address and transfer direction.
 If device is busy, use ack polling to wait until device is ready
 
 Input:   address and transfer direction of I2C device
*************************************************************************/
uint16_t i2c_start_wait(unsigned char address)
{
    uint8_t   twst;
    uint8_t  timeout = 10;


    while ( timeout-- > 0 )
    {
	    // send START condition
	    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    
    	// wait until transmission completed
    	I2C_wait_while(!(TWCR & (1<<TWINT)));
    
    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
    
    	// send device address
    	TWDR = address;
    	TWCR = (1<<TWINT) | (1<<TWEN);
    
    	// wail until transmission completed
    	I2C_wait_while(!(TWCR & (1<<TWINT)));
    
    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) ) 
    	{    	    
    	    /* device busy, send stop condition to terminate write operation */
	        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	        
	        // wait until stop condition is executed and bus released
	        I2C_wait_while(TWCR & (1<<TWSTO));
	        
    	    continue;
    	}
    	//if( twst != TW_MT_SLA_ACK) return 1;
    	break;
     }

     if (timeout)
     	return I2C_SUCCESS;
     else
     	return I2C_ERROR_TIMEOUT;

}/* i2c_start_wait */


/*************************************************************************
 Issues a repeated start condition and sends address and transfer direction 

 Input:   address and transfer direction of I2C device
 
 Return:  I2C_SUCCESS device accessible
          I2C_ERROR_* failed to access device
*************************************************************************/
uint16_t i2c_rep_start(unsigned char address)
{
    return i2c_start( address );

}/* i2c_rep_start */


/*************************************************************************
 Terminates the data transfer and releases the I2C bus
*************************************************************************/
uint16_t i2c_stop(void)
{
    /* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	
	// wait until stop condition is executed and bus released
	I2C_wait_while(TWCR & (1<<TWSTO));

	return I2C_SUCCESS;

}/* i2c_stop */


/*************************************************************************
  Send one byte to I2C device
  
  Input:    byte to be transfered
  Return:   I2C_SUCCESS write successful 
            I2C_ERROR_* write failed
*************************************************************************/
uint16_t i2c_write( unsigned char data )
{	
    uint8_t   twst;
    
	// send data to the previously addressed device
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	I2C_wait_while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK) return I2C_ERROR_WRITE;
	return I2C_SUCCESS;

}/* i2c_write */


/*************************************************************************
 Read one byte from the I2C device, request more data from device 
 
 Return:  byte read from I2C device
*************************************************************************/
uint16_t i2c_readAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	I2C_wait_while(!(TWCR & (1<<TWINT)));

    return (uint8_t) TWDR;

}/* i2c_readAck */


/*************************************************************************
 Read one byte from the I2C device, read is followed by a stop condition 
 
 Return:  byte read from I2C device
*************************************************************************/
uint16_t i2c_readNak(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	I2C_wait_while(!(TWCR & (1<<TWINT)));
	
    return (uint8_t) TWDR;

}/* i2c_readNak */
