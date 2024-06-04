#include "taskRtosUART0.h"
#include "semaphore.h"
#include "MACROS.h"
#include "FreeRTOS.h"
#include "task.h"

extern void taskRtosUART0_Rx(void) {

	for (;;) {
		if (semaphore_take(SEMAPHORE_RX_UART0, portMAX_DELAY)) {



		}
	}

	vTaskDelete(NULL);
	return;
}

extern void taskRtosUART0_Tx(void) {

	for (;;) {
		if (semaphore_take(SEMAPHORE_TX_UART0, portMAX_DELAY)) {

		}
	}

	vTaskDelete(NULL);
	return;
}
