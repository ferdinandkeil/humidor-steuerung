#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "lcd-routines.h"
#include "task.h"
#include "debounce.h"
#include "rotenc.h"
#include "menu.h"
#include "clock.h"
#include "fans.h"
#include "sensor.h"
#include "alarm.h"
#include "calibration.h"

#define LED PD6
#define BUZ PD7

/*
 Timer 1 COMP Interrupt
 Zeitmessung und Tasterentprellung
*/
ISR( TIMER1_COMPA_vect ) {
	encode_isr();
	tick();
}

/*
  Watchdog Task
  Setzt den Watchdog zurück
*/
static void Watchdog_run(void *_self, uint32_t now) {
	wdt_reset();
	Task_incRunTime(_self, 100);
}

Task_t* Watchdog(void) {
	wdt_enable(WDTO_500MS);		// Watchdog Timer aktivieren
	static Task_t watchdog;
	watchdog.canRun = TimedTask_canRun;
	watchdog.run = Watchdog_run;
	watchdog.runTime = 0;
	return &watchdog;
}

/*
  Main Routine
*/
int main(void) {
	// Watchdog deaktivieren
	wdt_reset();
	wdt_disable();
	
	// Ports konfigurieren
	DDRC = (1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3);

	DDRD = (1<<LED) | (1<<BUZ);
	PORTD = (1<<PD3) | (1<<PD4) | (1<<PD5);
	
	Time_now = 0;
	OCR1A = 12000;						// 12000 => alle 1ms
	TIMSK1 = (1<<OCIE1A);				// OCR Interrupt
	TCCR1B = (1<<WGM12) | (1<<CS10);	// CLK/1, CTC Mode
	
	TCCR0A = (1<<WGM01) | (1<<WGM00);  // non-inverted PWM output, Fast PWM (Mode 3)
	TCCR0B = (1<<CS02);    // Fast PWM (Mode 3), CLK/256
	OCR0A = 0;
	
	sei();
	
	Task_t *debounce = Debounce();
	Task_t *menu = Menu();
	Task_t *menuTimeOut = Menu_TimeOut();
	Task_t *clock = Clock();
	Task_t *fans = Fans();
	Task_t *sensor = Sensor();
	Task_t *alarm = Alarm();
	Task_t *watchdog = Watchdog();
	Calibration();
	
	Task_t *tasks[] = {debounce, menu, menuTimeOut, clock, sensor, alarm, fans, watchdog};
	
	TaskScheduler(tasks, sizeof(tasks)/sizeof(Task_t*));
	
	return 0;
}