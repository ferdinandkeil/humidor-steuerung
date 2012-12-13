#include <stdio.h>
#include <stdint.h>

int main(void) {
	uint16_t now = 0xFA00;
	uint16_t runTime = now;
	uint16_t rate = 0xFF;
	uint8_t inc = 2;

	uint16_t i;
	for (i = 0; i < 0xFFF; i += inc) {
		uint8_t canRun = (int16_t) (now - runTime) >= 0;
		if (canRun) {
			printf("now-runTime = %d ", (int16_t) (now - runTime));
			printf("now = %u runTime = %u ", now, runTime);
			runTime = runTime + rate;
			printf("next = %u\n", runTime);
		}
		now += inc;
	}
}
