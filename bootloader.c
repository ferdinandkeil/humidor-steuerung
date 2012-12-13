#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bootloader.h"
#include "menu.h"

int8_t Bootloader_State(int8_t input) {
	cli();
	_delay_ms(1000);
	wdt_enable(WDTO_15MS);
	_delay_ms(1000);
	return 0;
}