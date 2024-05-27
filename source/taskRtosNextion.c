#include "taskRtosNextion.h"
#include "MACROS.h"

extern void taskRtosNextion(void) {

	for (;;) {

		vTaskDelete(DELAY_250ms);
	}

	vTaskDelete(NULL);

	return;
}

