
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/


bool led = 0;
int count = 0, count_pwm = 0;
volatile unsigned int TimingDelay;
int count_micros = 0;

	
void TIM2_IRQHandler (void) { // 100us마다 진입, count++해줘야함

  if ((TIM2->SR & 0x0001) != 0) {                 // check interrupt source
		
		count++;
    TIM2->SR &= ~(1<<0);                          // clear UIF flag
 }
	
} // end TIM2_UP_IRQHandler





unsigned long micros_10us(){
	return count_micros;
}

void Delay(unsigned int nTime){
	TimingDelay = nTime;
	
	while(TimingDelay != 0);
}

void TimingDelay_Decrement(void){
	if(TimingDelay != 0x00){
		TimingDelay--;
	}
}

void SysTick_Handler(void)
{
   TimingDelay_Decrement();
	 count_micros++;
}
	





	
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  

	stm32_Init ();
	
	RCC->APB1ENR = 0x01;
	RCC->APB2ENR = 0x15;
	
	
	GPIOA->CRL &= (~(0x0f) <<20);
	GPIOA->CRL |= ((0x03) << 20);
	
	TIM2->CR1 = 0x01;
	TIM2->DIER = 0x01;
	TIM2->CNT = 0;
	TIM2->PSC = 72;
	TIM2->ARR = 100;
	
	NVIC->ICER[0] |= (0x01 << 25);     // timer1 clear
	NVIC->ISER[0] |= (0x01 << 28);     // timer2 update interrupt set
	
	
  for(;;) {
		
		if(count==10){
			GPIOA->ODR &= ~(0x01 << 5);
		}
		if(count == 100){
			GPIOA->ODR |= (0x01 << 5);
			count = 0;
		}
  }
   
}

	

/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
