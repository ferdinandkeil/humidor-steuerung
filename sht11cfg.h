// SHT11 Lib f�r AVR
// http://www.mikrocontroller.net/topic/145736#1705005

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>


// SHT11 hum/temp sensor


#define SHT11_AT_5V         4010
#define SHT11_AT_4V         3980
#define SHT11_AT_3_5V       3970
#define SHT11_AT_3V         3960
#define SHT11_AT_2_5V       3940

#define SHT11_TEMP_V_COMP   SHT11_AT_5V

#define SHT11_RES_LOW       1  //8_12_Bit
#define SHT11_RES_HIGH      0  //12_14_Bit
#define SHT11_RESOLUTION    SHT11_RES_LOW

#define SHT11_PORT	        C
#define SHT11_SCL	        (1<<PC5)
#define SHT11_SDA	        (1<<PC4)
#define SHT11_LOWRES	    SHT11_RESOLUTION

#define SHT11_CRC			1


#define GLUE(a, b)	a##b
#define PORT(x)		GLUE(PORT, x)
#define PIN(x)		GLUE(PIN, x)
#define DDR(x)		GLUE(DDR, x)

#define setBits(port,mask)	do{ (port) |=  (mask); }while(0)
#define clrBits(port,mask)	do{ (port) &= ~(mask); }while(0)
#define tstBits(port,mask)	((port) & (mask))

