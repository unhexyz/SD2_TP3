#include "mefNextion.h"
#include "nextion.h"
#include "fsl_debug_console.h"

static estMefNextion_enum estMefNextion;

extern void mefNextion_init(void) {
	estMefNextion = EST_NEXTION_pMAIN;

	return;
}

extern void mefNextion(void) {
	switch (estMefNextion) {
	case EST_NEXTION_RESET:
		PRINTF("No se detecto la pagina de la pantalla\r\n");
//		taskRtosNextion_error();

		estMefNextion = nextion_getPage();
		break;
	case EST_NEXTION_pMAIN:

		estMefNextion = nextion_getPage();
		break;
	case EST_NEXTION_pRADAR:

		for (int8_t angle=-60;angle<60;angle=angle+15){
			nextion_putObj(angle+60, 30, 1);
			taskRtosNextion_delay(500);
		}

		estMefNextion = nextion_getPage();
		break;
	case EST_NEXTION_pSERVO:

		estMefNextion = nextion_getPage();
		break;
	default:
		break;
	}

	return;
}
