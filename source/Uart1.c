#include "Uart1.h"
#include "MKL46Z4.h"
#include "fsl_uart.h"
#include "fsl_port.h"
#include "SD2_board.h"
#include "board.h"
#include "semaphore.h"
#include "semphr.h"
#include "FreeRTOS.h"
#include "fsl_uart_freertos.h"
#include "clock_config.h"
#include "pin_mux.h"
#include <string.h>

#define UART UART1
#define UART_CLKSRC BUS_CLK
#define UART_CLK_FREQ CLOCK_GetFreq(BUS_CLK)
#define UART_RX_TX_IRQn UART1_RX_TX_IRQn

/* Variables del entorno de uart_freertos */
const char *send_ring_overrun = "\r\nRing buffer overrun!\r\n";
const char *send_hardware_overrun = "\r\nHardware buffer overrun!\r\n";

uint8_t background_buffer[32];
uint8_t recv_buffer[4];
uart_rtos_handle_t handle;
struct _uart_handle t_handle;

uart_rtos_config_t uart_config = {
		.baudrate = 115200,
		.parity = kUART_ParityDisabled,
		.stopbits = kUART_OneStopBit,
		.buffer = background_buffer,
		.buffer_size = sizeof(background_buffer),
};

extern void Uart1_init(void) {
	uart_config.srcclk = UART_CLK_FREQ;
	uart_config.base = UART;

	UART_RTOS_Init(&handle, &t_handle, &uart_config);

	return;
}

extern void Uart1_send(uint8_t* to_send) {
	UART_RTOS_Send(&handle, to_send, strlen((char*)to_send));

	return;
}

extern void Uart1_read(uint8_t* receive) {
	size_t n;
	int error;

	error = UART_RTOS_Receive(&handle, receive, sizeof(receive), &n);

	/*
	 * Nota: deberiamos tener en cuenta los errores que pueden ocurrir
	 * segun el ejemplo dado, pero veremos depues.
	 * */
	if (error == kStatus_UART_RxHardwareOverrun) {
		/* Notify about hardware buffer overrun */
		if (kStatus_Success
				!= UART_RTOS_Send(&handle, (uint8_t*) send_hardware_overrun,
						strlen(send_hardware_overrun))) {
			vTaskSuspend(NULL);
		}
	}
	if (error == kStatus_UART_RxRingBufferOverrun) {
		/* Notify about ring buffer overrun */
		if (kStatus_Success
				!= UART_RTOS_Send(&handle, (uint8_t*) send_ring_overrun,
						strlen(send_ring_overrun))) {
			vTaskSuspend(NULL);
		}
	}

	return;
}

//extern void UART1_IRQHandler(void) {
//	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//
//	if ((kUART_RxDataRegFullFlag) & UART_GetStatusFlags(UART_TYPE)
//			&& (kUART_RxDataRegFullInterruptEnable)
//					& UART_GetEnabledInterrupts(UART_TYPE)) {
//		/* obtiene dato recibido por puerto serie */
//		byteRec = UART_ReadByte(UART_TYPE);
//		dataAvailable = true;
//		UART_ClearStatusFlags(UART_TYPE, kUART_RxDataRegFullFlag);
//	}
//
//	if ((kUART_TxDataRegEmptyFlag) & UART_GetStatusFlags(UART_TYPE)
//			&& (kUART_TxDataRegEmptyInterruptEnable)
//					& UART_GetEnabledInterrupts(UART_TYPE)) {
//		/* entra acá cuando se se puede poner un nuevo byte en el buffer
//		 * de transmición
//		 */
//		semaphore_giveFromISR(SEMAPHORE_TX_UART1, &xHigherPriorityTaskWoken);
//
//		UART_DisableInterrupts(UART_TYPE, kUART_TxDataRegEmptyInterruptEnable);
//		UART_ClearStatusFlags(UART_TYPE, kUART_TxDataRegEmptyFlag);
//	}
//
//	if ((kUART_TransmissionCompleteFlag) & UART_GetStatusFlags(UART_TYPE)
//			&& (kUART_TransmissionCompleteInterruptEnable)
//					& UART_GetEnabledInterrupts(UART_TYPE)) {
//		/* entra acá cuando se completó la transmición del byte
//		 *
//		 */
//		UART_DisableInterrupts(UART_TYPE,
//				kUART_TransmissionCompleteInterruptEnable);
//		UART_ClearStatusFlags(UART_TYPE, kUART_TransmissionCompleteFlag);
//	}
//
//	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//}
