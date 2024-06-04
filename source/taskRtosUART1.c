#include "taskRtosUART1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "MACROS.h"
#include "semaphore.h"

extern void taskRtosUART1_Tx(void *pvParameters) {

	for (;;) {
//		if (semaphore_take(SEMAPHORE_TX_UART1, portMAX_DELAY)) {
//
//		}
		vTaskSuspend(NULL);
	}

	vTaskDelete(NULL);

	return;
}
