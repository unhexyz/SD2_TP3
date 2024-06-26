#include "taskRtos.h"

#include "FreeRTOS.h"
#include "task.h"

#include "taskRtosUART0.h"
#include "taskRtosNextion.h"
#include "taskRtosPERIFERICOS.h"
#include "fsl_debug_console.h"

#define UART0_PRIORITY		configMAX_PRIORITIES - 1
#define NEXTION_PRIORITY		tskIDLE_PRIORITY + 1
#define PERIFERICOS_PRIORITY	tskIDLE_PRIORITY + 1
#define STACK_SIZE_GENERAL		configMINIMAL_STACK_SIZE+100

typedef struct {
	TaskFunction_t taskFunction;
	const char *taskName;
	uint16_t stackDepth;
	UBaseType_t priority;
} TaskConfig_t;

static const TaskConfig_t taskConfigs[NUM_TASK] = {
	{taskRtosUART0, "ISR_RX_U0", STACK_SIZE_GENERAL, UART0_PRIORITY},
    //{taskRtosNextion, "ISR_TX_U1", STACK_SIZE_GENERAL, NEXTION_PRIORITY},
    //{taskRtosPERIFERICOS_Sensor, "Sensor", STACK_SIZE_GENERAL, PERIFERICOS_PRIORITY},
	//{taskRtosPERIFERICOS_Servo, "Servo", STACK_SIZE_GENERAL, PERIFERICOS_PRIORITY},
};

extern void taskRtos_create(void) {
	for (int i = 0; i < NUM_TASK; i++) {
		xTaskCreate(taskConfigs[i].taskFunction, taskConfigs[i].taskName,
				taskConfigs[i].stackDepth,
				NULL, taskConfigs[i].priority,
				NULL);
	}

	return;
}

