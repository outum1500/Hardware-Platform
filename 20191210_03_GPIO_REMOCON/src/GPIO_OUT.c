


#include "func.h"
#include "user.h"




/*----------------------------------------------------------*
*  MIAN ENTRY												|
*----------------------------------------------------------*/
///////////////////////////////////////////////
int main(void) 
{
	Init_SysTick();
	Init_UART1();
//	port_init();

	setup();

	while(1) 
		loop();

//	return 0;
}
///////////////////////////////////////////////
/*----------------------------------------------------------*\
 | END OF FILE												|
\*----------------------------------------------------------*/














