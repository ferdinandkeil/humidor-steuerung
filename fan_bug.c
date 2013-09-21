#include <stdio.h>
#include <stdint.h>
#include <task.h>

// TimedTask
// Determine if a task can run right now.
uint8_t TimedTask_canRun(uint8_t runTime, uint8_t now) {
	return (int8_t) (now - runTime) >= 0;
}

int main(void) {
	
}