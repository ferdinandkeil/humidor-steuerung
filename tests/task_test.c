#include <stdio.h>
#include <stdint.h>

// TimedTask
// Determine if a task can run right now.
uint8_t TimedTask_canRun(uint8_t runTime, uint8_t now) {
	return (int8_t) (now - runTime) >= 0;
}

int main(void) {
	uint8_t t, i;
	uint8_t runTime = 0;
	for (i = 0; i < 4; i++) {
		for (t = 3; t < 255; t++) {
			if (TimedTask_canRun(runTime, t)) {
				printf("t: %3u runTime: %3u\n", t, runTime);
				runTime += 10;
			}
			if (rand() % 100 > 75)
				t += 3;
		}
	}
}