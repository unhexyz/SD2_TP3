#include "MG90S.h"
#include "pwm.h"
#include "MACROS.h"
#include "FreeRTOS.h"
#include "task.h"

extern void MG90S_init(void) {
	pwm_init();
	MG90S_test();

	return;
}
extern void MG90S_setAngle(uint8_t angle) {
	float dutyCycle = 5.0 + (3.35 * (angle / 120.0));

	if (angle > 120)
		angle = 120;

	pwm_setDuty((uint8_t) dutyCycle);

	return;
}
extern void MG90S_test(void) {
	for (uint8_t angle = 0; angle <= 120; angle += 10) {
		MG90S_setAngle(angle);
		vTaskDelay(DELAY_250ms);
	}

	for (int8_t angle = 120; angle >= 0; angle -= 10) {
		MG90S_setAngle(angle);
		vTaskDelay(DELAY_250ms);
	}

	return;
}

