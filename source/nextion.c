#include "nextion.h"
#include "Uart1.h"

static const char cmd_ADD = {"add"};
static const char cmd_RESET = {"rest"};


static void nextion_EndTrama(void);

extern void nextion_init(void) {

	return;
}

static void nextion_EndTrama(void) {

	return;
}

extern void nextion_reset(void){
	while(1){
		if(uart1)
	}

	return;
}

static void nextion_EndTrama(void) {
	Uart1_sendByte(0xFF);
	Uart1_sendByte(0xFF);
	Uart1_sendByte(0xFF);

	return;
}
