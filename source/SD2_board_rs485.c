/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, Gustavo Muro
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*==================[inclusions]=============================================*/
#include "board.h"
#include "MKL46Z4.h"
#include "fsl_uart.h"
#include "fsl_port.h"
#include "SD2_board.h"

/*==================[macros and definitions]=================================*/

#define RS485_UART                  UART1
#define RS485_UART_IRQn             UART1_IRQn
#define RS485_kSimClockGateUart     kSimClockGateUart1

/*==================[internal data declaration]==============================*/
static const board_gpioInfo_type board_gpioContLine[] =
{
    {PORTA, GPIOA, 16},    /* RE */
    {PORTA, GPIOA, 17},    /* DE */
};

static bool dataAvailable;
static uint8_t byteRec;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void rs485_RE(bool est)
{
    if (est)
    	GPIO_PortSet(board_gpioContLine[0].gpio, 1<<board_gpioContLine[0].pin);
    else
    	GPIO_PortClear(board_gpioContLine[0].gpio, 1<<board_gpioContLine[0].pin);
}

static void rs485_DE(bool est)
{
    if (est)
    	GPIO_PortSet(board_gpioContLine[1].gpio, 1<<board_gpioContLine[1].pin);
    else
    	GPIO_PortClear(board_gpioContLine[1].gpio, 1<<board_gpioContLine[1].pin);
}

/*==================[external functions definition]==========================*/
void board_rs485_init(void)
{
	int32_t i;
	gpio_pin_config_t gpio_config =
	{
		.outputLogic = 1,
		.pinDirection = kGPIO_DigitalOutput,
	};

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
		.mux = kPORT_MuxAsGpio,
	};

	uart_config_t config;

    /* inicialización de pines de control */
    for (i = 0 ; i < 2 ; i++)
	{
		PORT_SetPinConfig(board_gpioContLine[i].port, board_gpioContLine[i].pin, &port_config);
		GPIO_PinInit(board_gpioContLine[i].gpio, board_gpioContLine[i].pin, &gpio_config);
	}

    rs485_RE(false);
    rs485_DE(false);

    /*
	 * config.baudRate_Bps = 115200U;
	 * config.parityMode = kUART_ParityDisabled;
	 * config.stopBitCount = kUART_OneStopBit;
	 * config.txFifoWatermark = 0;
	 * config.rxFifoWatermark = 1;
	 * config.enableTx = false;
	 * config.enableRx = false;
	 */
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = 115200;
	config.enableTx = true;
	config.enableRx = true;

	UART_Init(RS485_UART, &config, CLOCK_GetFreq(BUS_CLK));

	/* Configura los pines RX y TX de la UART1 */
	PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);
	PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3);

    dataAvailable = false;

    UART_EnableInterrupts(RS485_UART, kUART_RxDataRegFullInterruptEnable);
    UART_EnableInterrupts(RS485_UART, kUART_TxDataRegEmptyInterruptEnable);
    UART_EnableInterrupts(RS485_UART, kUART_TransmissionCompleteInterruptEnable);

    NVIC_EnableIRQ(RS485_UART_IRQn);
}

void board_rs485_sendByte(uint8_t dato)
{
    rs485_RE(true);
    rs485_DE(true);

    UART_WriteByte(RS485_UART, dato);

    UART_EnableInterrupts(RS485_UART, kUART_TxDataRegEmptyInterruptEnable);
    UART_EnableInterrupts(RS485_UART, kUART_TransmissionCompleteInterruptEnable);

}

bool board_rs485_isDataAvailable(void)
{
    return dataAvailable;
}

uint8_t board_rs485_readByte(void)
{
    dataAvailable = false;
    return byteRec;
}

void UART1_IRQHandler(void)
{
	if ( (kUART_RxDataRegFullFlag)            & UART_GetStatusFlags(RS485_UART) &&
	     (kUART_RxDataRegFullInterruptEnable) & UART_GetEnabledInterrupts(RS485_UART) )
	{
		/* obtiene dato recibido por puerto serie */
		byteRec = UART_ReadByte(RS485_UART);
		dataAvailable = true;
		UART_ClearStatusFlags(RS485_UART, kUART_RxDataRegFullFlag);
	}

	if ( (kUART_TxDataRegEmptyFlag)            & UART_GetStatusFlags(RS485_UART) &&
		 (kUART_TxDataRegEmptyInterruptEnable) & UART_GetEnabledInterrupts(RS485_UART) )
    {
        /* entra acá cuando se se puede poner un nuevo byte en el buffer
         * de transmición
         */
		UART_DisableInterrupts(RS485_UART, kUART_TxDataRegEmptyInterruptEnable);
		UART_ClearStatusFlags(RS485_UART, kUART_TxDataRegEmptyFlag);
    }

	if ( (kUART_TransmissionCompleteFlag)            & UART_GetStatusFlags(RS485_UART) &&
		 (kUART_TransmissionCompleteInterruptEnable) & UART_GetEnabledInterrupts(RS485_UART) )
    {
        /* entra acá cuando se completó la transmición del byte
         *
         */
		UART_DisableInterrupts(RS485_UART, kUART_TransmissionCompleteInterruptEnable);
		UART_ClearStatusFlags(RS485_UART, kUART_TransmissionCompleteFlag);
        rs485_RE(false);
        rs485_DE(false);
    }
}

/*==================[end of file]============================================*/
