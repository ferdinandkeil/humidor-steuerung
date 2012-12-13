#include <stdio.h>
#include <stdint.h>

int main(void) {
	uint8_t t, j;
	for (j = 0; j < 255; j++) {
		printf("t %% %u: ", j);
		for (t = 0; t < 255; t++) {
			if ((t & j))
				printf("-");
			else
				printf("_");
		}
		printf("\n\n");
	}
}