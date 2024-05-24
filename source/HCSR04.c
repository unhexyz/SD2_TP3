#include "HCSR04.h"


#define HCSR04_TRIGGER_PORT PORTE
#define HCSR04_TRIGGER_GPIO GPIOE
#define HCSR04_TRIGGER_PIN 30

#define HCSR04_ECHO_PORT PORTC
#define HCSR04_ECHO_GPIO GPIOC
#define HCSR04_ECHO_PIN 8


extern void HCSR04_init(){

	int32_t i;

	// Configuracion del trigger
	gpio_pin_config_t gpio_trigger_config =
	{
		.outputLogic = 1,
		.pinDirection = kGPIO_DigitalOutput,
	};

	const port_pin_config_t port_trigger_config = {
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

	PORT_SetPinConfig(HCSR04_TRIGGER_PORT, HCSR04_TRIGGER_PIN, &port_trigger_config);
	GPIO_PinInit(HCSR04_TRIGGER_GPIO, HCSR04_TRIGGER_PIN, &gpio_trigger_config);


	// Configuracion del echo
	gpio_pin_config_t gpio_echo_config = {
		.pinDirection = kGPIO_DigitalInput,
		.outputLogic = 1,
	};

	const port_pin_config_t port_echo_config = {
		/* Internal pull-up resistor is enabled */
		.pullSelect = kPORT_PullDown,
		/* Fast slew rate is configured */
		.slewRate = kPORT_FastSlewRate,
		/* Passive filter is disabled */
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		/* Low drive strength is configured */
		.driveStrength = kPORT_LowDriveStrength,
		/* Pin is configured as PTC3 */
		.mux = kPORT_MuxAsGpio,
	};

	PORT_SetPinConfig(HCSR04_ECHO_PORT, HCSR04_TRIGGER_PIN, &port_trigger_config);
	GPIO_PinInit(HCSR04_EC_GPIO, HCSR04_TRIGGER_PIN, &gpio_trigger_config);

	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);

	/* inicialización de leds */
	for (i = 0 ; i < BOARD_LED_ID_TOTAL ; i++)
	{
		PORT_SetPinConfig(board_gpioLeds[i].port, board_gpioLeds[i].pin, &port_led_config);
		GPIO_PinInit(board_gpioLeds[i].gpio, board_gpioLeds[i].pin, &gpio_led_config);
	}

	/* inicialización de SWs */
	for (i = 0 ; i < BOARD_SW_ID_TOTAL ; i++)
	{
		PORT_SetPinConfig(board_gpioSw[i].port, board_gpioSw[i].pin, &port_sw_config);
		GPIO_PinInit(board_gpioSw[i].gpio, board_gpioSw[i].pin, &gpio_sw_config);
	}

}


extern void HCSR04_disparar(void){

	echo_flag = 0;

	trigger_flag = 1;

}

extern uint32_t HCSR04_ultimaMedicion(void);

extern void HCSR04_task1ms();
