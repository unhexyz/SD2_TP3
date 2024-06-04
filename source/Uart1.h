#ifndef UART1_H_
#define UART1_H_

#include <stdint.h>
#include <stdbool.h>

extern void Uart1_init(void);
extern void Uart1_send(uint8_t* to_send);
extern void Uart1_read(uint8_t* receive);

#endif /* UART1_H_ */
