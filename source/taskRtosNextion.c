#include "taskRtosNextion.h"
#include "MACROS.h"
#include "Uart1.h"
#include "mefNextion.h"
#include "task.h"

extern void taskRtosNextion(void) {
	Uart1_init();
	mefNextion_init();

	vTaskDelay(DELAY_3s);

	for (;;) {
		mefNextion();

		vTaskDelay(DELAY_100ms);
	}

	vTaskDelete(NULL);

	return;
}

extern void taskRtosNextion_error(void) {
	vTaskSuspend(NULL);

	return;
}

extern void taskRtosNextion_delay(uint16_t delay) {
	vTaskDelay(pdMS_TO_TICKS(delay));

	return;
}

