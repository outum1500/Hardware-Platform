
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/


bool led = 0;
int count = 0, pwm_duty = 0;
int count_micros = 0;
unsigned long c_micro = 0, p_micro = 0;

   
void TIM2_IRQHandler (void) { // 20us마다 진입, count++해줘야함

	if((TIM2->SR & 0x0001) != 0){  // Update Interrupt 
	
		count++;
		
		if(count==100){  // 2ms
			count = 0;
			GPIOA->ODR |= (0x01 << 5);
		}
		else if(count == pwm_duty) {
			GPIOA->ODR &= ~(0x01 << 5);
		}
		
		TIM2->SR &= ~(1<<0);
	}
	
  if ((TIM2->SR & 0x0002) != 0) {                 // Capture/Compare Interrupt
   

    TIM2->SR &= ~(1<<1);                          // clear Capture/Compare flag
 }
   
} // end TIM2_UP_IRQHandler





unsigned int TimingDelay;
void Delay(unsigned int nTime)
{
    TimingDelay = nTime;
    while(TimingDelay != 0);
}
void TimingDelay_Decrement(void)
{
    if(TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
   count_micros++;
}
unsigned long millis(){
	return count_micros;
}
   





   
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  

  stm32_Init ();
  
	RCC->APB1ENR |= (0x01 << 0);
	
  TIM2->CR1 = 0x0001;  // Counter enable
	TIM2->DIER = 0x0003;  // Update Interrupt enable
	TIM2->CNT = 0;
	TIM2->ARR = 20;  // 20us Interrupt
	TIM2->PSC = 72;  // 1us 마다 CNT++
	
	NVIC->ICER[0] |= (0x01 << 25);
	NVIC->ISER[0] |= (0x01 << 28);  // TIM2 enable
	
	GPIOA->CRL = 0x00300000;  // Porta.5 50MHz output push-pull
	
	SysTick->CTRL = 0x00000006;
	SysTick->LOAD = 72000;  // 1ms
	SysTick->CTRL = 0x00000007;
   
   
  for(;;) {
      
		c_micro = millis();
		
		if(c_micro - p_micro > 10){  // 10ms마다 진입
			p_micro = c_micro;
			pwm_duty++;
			
			if(pwm_duty >= 100) pwm_duty = 0;
		}
      
  }
   
}

   

/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/