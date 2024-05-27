#include "mefSensor.h"

typedef enum {
	EST_SENSOR_RESET = 0, EST_SENSOR_ENABLE, EST_SENSOR_DISABLE,
} estMefSensor_enum;

static estMefSensor_enum estMefSensor;

extern void mefSensor_init(void) {
	estMefSensor = EST_SENSOR_RESET;

	return;
}

extern void mefSensor(void) {
	switch (estMefSensor) {
	case EST_SENSOR_RESET:
		/*Acciones de reset*/

		estMefSensor = EST_SENSOR_ENABLE;
		break;
	case EST_SENSOR_ENABLE:
		/*Acciones de enable*/


		break;
	case EST_SENSOR_DISABLE:
		/*Acciones de disable*/

		break;
	default:
		break;
	}

	return;
}
