
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/


bool led = 0;
	
void EXTI15_10_IRQHandler(void)
{
  if (EXTI->PR & (1<<13)) {                       // EXTI0 interrupt pending?
    if (led == 1) {
    	GPIOA->ODR &= ~(0x01 << 5);                           // switch on LED
			led = 0;
		} else {
    	GPIOA->ODR |=  (0x01 << 5);                           // switch off LED
			led = 1;
		}
    EXTI->PR = (1<<13);                          // clear pending interrupt
  }
}
	
	
	
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  

	stm32_Init ();
	
	RCC->APB2ENR = 0x15;
	
	
	GPIOA->CRL &= (~(0x0f) <<20);
	GPIOA->CRL |= ((0x03) << 20);
	
	EXTI->FTSR = 0x01 << 13;
	EXTI->IMR = 0x01 << 13;
	
	AFIO->EXTICR[3] = 0X02<<4;
	
	
	
  for(;;) {
		

		
  }
   
}

	

/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
