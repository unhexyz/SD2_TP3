#include "taskRtosNextion.h"
#include "MACROS.h"

extern void taskRtosNextion(void) {
	mefNextion_init();

	for (;;) {
		mefNextion();

		vTaskDelete(DELAY_250ms);
	}

	vTaskDelete(NULL);

	return;
}

