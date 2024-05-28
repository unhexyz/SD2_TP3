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
extern void MG90S_setAngle(int16_t angle) {
//	float dutyCycle = 5.0 + (3.35 * (angle / 120.0));
	float conv_lineal = (angle+90)*1/180.0+1;
	float dutyCycle = conv_lineal/0.2;

	pwm_setDuty((uint8_t) dutyCycle);

	return;
}
extern void MG90S_test(void) {
//	for (uint8_t angle = 0; angle <= 120; angle += 10) {
//		MG90S_setAngle(angle);
//		vTaskDelay(DELAY_500ms);
//	}
//
//	for (int8_t angle = 120; angle >= 0; angle -= 10) {
//		MG90S_setAngle(angle);
//		vTaskDelay(DELAY_250ms);
//	}
	MG90S_setAngle(-90);	// 1ms -90 grados
	vTaskDelay(DELAY_2s);
	pwm_setDuty(0);	// 1.5ms 0 grados
	vTaskDelay(DELAY_2s);
	pwm_setDuty(90); //2ms son 90 grados
	vTaskDelay(DELAY_2s);

	return;
}

