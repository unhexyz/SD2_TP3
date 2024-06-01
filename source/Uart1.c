#include "Uart1.h"
#include "MKL46Z4.h"
#include "fsl_uart.h"
#include "fsl_port.h"
#include "SD2_board.h"
#include "board.h"
#include "semaphore.h"
#include "semphr.h"
#include "FreeRTOS.h"

#define UART_TYPE				UART1
#define UART_IRQn				UART1_IRQn
#define UART_kSimClockGateUart	kSimClockGateUart1

static bool dataAvailable;
static uint8_t byteRec;

extern void Uart1_init(void) {
	int32_t i;
	gpio_pin_config_t gpio_config = { .outputLogic = 1, .pinDirection =
			kGPIO_DigitalOutput, };

	const port_pin_config_t port_config = {
	/* Internal pull-up/down resistor is disabled */
	.pullSelect = kPORT_PullDisable,
	/* Slow slew rate is configured */
	.slewRate = kPORT_SlowSlewRate,
	/* Passive filter is disabled */
	.passiveFilterEnable = kPORT_PassiveFilterDisable,
	/* Low drive strength is configured */
	.driveStrength = kPORT_LowDriveStrength,
	/* Pin is configured as PTC3 */
	.mux = kPORT_MuxAsGpio, };

	uart_config_t config;

	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = 115200;
	config.enableTx = true;
	config.enableRx = true;

	UART_Init(UART_TYPE, &config, CLOCK_GetFreq(BUS_CLK));

	/* Configura los pines RX y TX de la UART1 */
	PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);
	PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3);

	dataAvailable = false;

	UART_EnableInterrupts(UART_TYPE, kUART_RxDataRegFullInterruptEnable);
	UART_EnableInterrupts(UART_TYPE, kUART_TxDataRegEmptyInterruptEnable);
	UART_EnableInterrupts(UART_TYPE, kUART_TransmissionCompleteInterruptEnable);

	NVIC_EnableIRQ(UART_IRQn);

	return;
}

extern void Uart1_sendByte(uint8_t dato) {
	UART_WriteByte(UART_TYPE, dato);

	UART_EnableInterrupts(UART_TYPE, kUART_TxDataRegEmptyInterruptEnable);
	UART_EnableInterrupts(UART_TYPE, kUART_TransmissionCompleteInterruptEnable);

	return;
}

extern bool Uart1_isDataAvaible(void) {
	return dataAvailable;
}

extern uint8_t Uart1_readByte(void) {
	dataAvailable = false;

	return byteRec;
}

extern void UART1_IRQHandler(void) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if ((kUART_RxDataRegFullFlag) & UART_GetStatusFlags(UART_TYPE)
			&& (kUART_RxDataRegFullInterruptEnable)
					& UART_GetEnabledInterrupts(UART_TYPE)) {
		/* obtiene dato recibido por puerto serie */
		byteRec = UART_ReadByte(UART_TYPE);
		dataAvailable = true;
		UART_ClearStatusFlags(UART_TYPE, kUART_RxDataRegFullFlag);
	}

	if ((kUART_TxDataRegEmptyFlag) & UART_GetStatusFlags(UART_TYPE)
			&& (kUART_TxDataRegEmptyInterruptEnable)
					& UART_GetEnabledInterrupts(UART_TYPE)) {
		/* entra acá cuando se se puede poner un nuevo byte en el buffer
		 * de transmición
		 */
		semaphore_giveFromISR(SEMAPHORE_TX_UART1, &xHigherPriorityTaskWoken);

		UART_DisableInterrupts(UART_TYPE, kUART_TxDataRegEmptyInterruptEnable);
		UART_ClearStatusFlags(UART_TYPE, kUART_TxDataRegEmptyFlag);
	}

	if ((kUART_TransmissionCompleteFlag) & UART_GetStatusFlags(UART_TYPE)
			&& (kUART_TransmissionCompleteInterruptEnable)
					& UART_GetEnabledInterrupts(UART_TYPE)) {
		/* entra acá cuando se completó la transmición del byte
		 *
		 */
		UART_DisableInterrupts(UART_TYPE,
				kUART_TransmissionCompleteInterruptEnable);
		UART_ClearStatusFlags(UART_TYPE, kUART_TransmissionCompleteFlag);
	}

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
