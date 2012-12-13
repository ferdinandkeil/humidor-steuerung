#include "debounce.h"

static void Debounce_run(void *_self, uint32_t now) {
	key_press_isr();
	Task_incRunTime(_self, 10);
}

Task_t* Debounce(void) {
	encode_init();
	static Task_t debounce;
	debounce.canRun = TimedTask_canRun;
	debounce.run = Debounce_run;
	debounce.runTime = 0;
	return &debounce;
}