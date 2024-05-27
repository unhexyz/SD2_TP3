#include "mefNextion.h"

typedef enum {
	EST_NEXTION_RESET = 0, EST_NEXTION_DATA,
} estMefNextion_enum;

static estMefNextion_enum estMefNextion;

extern void mefNextion_init(void) {
	estMefNextion = EST_NEXTION_RESET;

	return;
}

extern void mefNextion(void) {
	switch (estMefNextion) {
	case EST_NEXTION_RESET:

		break;
	case EST_NEXTION_DATA:

		break;
	default:
		break;
	}

	return;
}
