#ifndef MMA8451_H_
#define MMA8451_H_

/*==================[inclusions]=============================================*/
#include <stdint.h>
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/


// Almacena la ultima medicion hecha.
static uint32_t medicion = 0;

// Se pone en 0 cuando se genera un disparo del sensor.
// Se pone en 1 cuando se completa la lectura de la senal 'echo'.
static uint8_t echo_flag = 0;

// Se pone en 1 cuando se elevo el pulso de trigger.
// Se pone en 0 cuando ya termino el pulso.
static uint8_t trigger_flag = 0;


/*==================[external functions definition]==========================*/

/** \brief Dispara un pulso de disparo sobre el sensor.
 **
 **/

extern void HCSR04_init();

extern void HCSR04_disparar(void);


/** \brief Devuelve la ultima medicion detectada por el sensor
 **
 ** \return La ultima medicion detectada por el sensor
 **
 **/

extern uint32_t HCSR04_ultimaMedicion(void);


/** \brief Funcion que debe ejecutarse cada 1ms.
 **
 **/

extern void HCSR04_task1ms();

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MMA8451_H_ */
