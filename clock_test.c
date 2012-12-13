#include <stdio.h>
#include <stdint.h>

#define TIME_END 0xFFFFFFF
#define TIME_STEP 333
#define RATE 200

int main(void) {
	uint32_t now = 0;
	uint32_t runTime = now;
	
	printf("Beginn\n");
	while (now < TIME_END) {
		uint8_t canRun = (int32_t) (now - runTime) >= 0;
		if (canRun) {
			static uint32_t cum;
			static uint32_t last;
			static uint32_t s;
			
			if (now - last >= 1000) {
				s++;
				last = now;
			}
			/*cum += now - last;
			if (cum >= 1000) {
				cum -= 1000;
				s++;
//				printf("now = %u ", now);
//				printf("s = %u ", s);
//				printf("\n");
			}
			last = now;*/
//			printf("now-runTime = %d ", (int16_t) (now - runTime));
//			printf("now = %u ", now);
//			printf("runTime = %u ", runTime);
//			printf("s = %u ", s);
//			printf("cum = %u ", cum);
//			printf("\n");
			if (s != now/1000) {
				printf("Fehler ");
				printf("now = %u ", now);
				printf("s = %u ", s);
				printf("\n");
				break;
			}
			runTime = runTime + RATE;
		}
		now += TIME_STEP;
	}
	printf("Ende\n");
}
