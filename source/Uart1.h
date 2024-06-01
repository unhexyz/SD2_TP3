#ifndef UART1_H_
#define UART1_H_

#include <stdbool.h>

extern void Uart1_sendByte(uint8_t dato);
extern uint8_t Uart1_readByte(void);
extern bool Uart1_isDataAvaible(void);
extern void Uart1_init(void);

#endif /* UART1_H_ */
