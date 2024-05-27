#include "mefServo.h"
#include "MG90S.h"

typedef enum {
	EST_SERVO_RESET = 0, EST_SERVO_RUNNING, EST_SERVO_STOP,
} estMefServo_enum;

static estMefServo_enum estMefServo;

extern void mefServo_init(void) {
	estMefServo = EST_SERVO_RESET;
	MG90S_init();

	return;
}

extern void mefServo(void) {
	switch (estMefServo) {
	case EST_SERVO_RESET:

		break;
	case EST_SERVO_RUNNING:

		break;
	case EST_SERVO_STOP:

		break;
	default:
		break;
	}

	return;
}
