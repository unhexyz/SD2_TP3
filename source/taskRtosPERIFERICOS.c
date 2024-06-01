#include "taskRtosPERIFERICOS.h"
#include "MACROS.h"
#include "mefSensor.h"
#include "mefServo.h"
#include "FreeRTOS.h"
#include "task.h"

extern void taskRtosPERIFERICOS_Servo(void) {
	mefServo_init();

	for (;;) {
		mefServo();

		vTaskDelay(DELAY_100ms);
	}

	vTaskDelete(NULL);

	return;
}

extern void taskRtosPERIFERICOS_Sensor(void) {
	mefSensor_init();

	for (;;) {
		mefSensor();

		vTaskDelay(DELAY_50ms);
	}

	vTaskDelete(NULL);

	return;
}

extern void taskRtosPERIFERICOS_delayServo(void) {
	vTaskDelay(DELAY_250ms);

	return;
}
