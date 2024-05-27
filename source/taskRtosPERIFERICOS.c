#include "taskRtosPERIFERICOS.h"
#include "MACROS.h"
#include "mefSensor.h"
#include "mefServo.h"
#include "FreeRTOS.h"
#include "task.h"

extern void taskRtosPERIFERICOS(void) {
//	mefSensor_init();
	mefServo_init();

	for (;;) {
//		mefSensor();
		mefServo();

		vTaskDelay(DELAY_100ms);
	}

	vTaskDelete(NULL);

	return;
}
