
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization
   
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/




int toggle1 = 0;
int toggle2 = 0;




void EXTI1_IRQHandler(void)
{
   if (EXTI->PR & (1<<1)) {                       // EXTI0 interrupt pending?
      if(toggle1)
         GPIOA->ODR |= 0x01 << 11;
      else 
         GPIOA->ODR &= ~(0x01 << 11);
      
      toggle1 = !toggle1;
      
      //EXTI->PR = (1<<1);                          // clear pending interrupt
   }
}


void EXTI2_IRQHandler(void)
{
   if (EXTI->PR & (1<<2)) {                       // EXTI0 interrupt pending?
      if(toggle2)
         GPIOA->ODR |= 0x01 << 12;
      else 
         GPIOA->ODR &= ~(0x01 << 12);
      toggle2 = !toggle2;
      EXTI->PR = (1<<2);                          // clear pending interrupt
   }
}







int main (void) {
  

   stm32_Init ();
   
   //RCC->APB2ENR = 0x0C;
   
   
   
  for(;;) {
      
    }
   
  }


   
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/