
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/


bool led = 0;
int count = 0, pwm_duty = 0, motor_duty = 70;
int motor_duty2 = 70;
int motor_count = 0;
int count_micros = 0;
unsigned long c_micro = 0, p_micro = 0;
unsigned long c_micro2 = 0, p_micro2 = 0;

   
void TIM2_IRQHandler (void) { // 2.5ms마다 진입, count++해줘야함

	if((TIM2->SR & 0x0001) != 0){  // Update Interrupt 
		
		
		count++;
		
		if(count == 9) count = 0;
		else if(count == 1) {
			GPIOA->ODR |= (0x01 << 5);
			TIM2->CCR1 = motor_duty;
		}
		else if(count == 2){
			GPIOA->ODR |= (0x01 << 6);
			TIM2->CCR1 = motor_duty2;
		}
		else if(count == 3)
			GPIOA->ODR |= (0x01 << 7);
		else if(count == 4)
			GPIOA->ODR |= (0x01 << 8);
		else if(count == 5)
			GPIOA->ODR |= (0x01 << 9);
		else if(count == 6)
			GPIOA->ODR |= (0x01 << 10);
		else if(count == 7)
			GPIOA->ODR |= (0x01 << 11);
		else if(count == 8)
			GPIOA->ODR |= (0x01 << 12);
		
		
		
		TIM2->SR &= ~(1<<0);
		
	}
	
  if ((TIM2->SR & 0x0002) != 0) {                 // Capture/Compare Interrupt
   
		if(count == 1)
			GPIOA->ODR &= ~(0x01 << 5);
		if(count == 2)
			GPIOA->ODR &= ~(0x01 << 6);
		if(count == 3)
			GPIOA->ODR &= ~(0x01 << 7);
		if(count == 4)
			GPIOA->ODR &= ~(0x01 << 8);
		if(count == 5)
			GPIOA->ODR &= ~(0x01 << 9);
		if(count == 6)
			GPIOA->ODR &= ~(0x01 << 10);
		if(count == 7)
			GPIOA->ODR &= ~(0x01 << 11);
		if(count == 8)
			GPIOA->ODR &= ~(0x01 << 12);
		
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
unsigned long micros_10(){
	return count_micros;
}
   





   
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  

  stm32_Init ();
  
	RCC->APB1ENR |= (0x01 << 0);
	
  TIM2->CR1 = 0x0001;  // Counter enable
	TIM2->DIER = 0x0003;  // Update Interrupt, Capture/Compare Interrupt enable
	TIM2->CCMR1 = 0x0060;  // PWM Mode 1
	TIM2->CNT = 0;
	TIM2->ARR = 250;  // 2.5ms Interrupt
	TIM2->PSC = 720;  // 10us 마다 CNT++
	TIM2->CCR1 = 70;  // 0.7ms
	
	NVIC->ICER[0] |= (0x01 << 25);
	NVIC->ISER[0] |= (0x01 << 28);  // TIM2 enable
	
	GPIOA->CRL = 0x33300000;  // Porta.5 50MHz output push-pull
	GPIOA->CRH = 0x33333333;
	
	SysTick->CTRL = 0x00000006;
	SysTick->LOAD = 720;  // 10us
	SysTick->CTRL = 0x00000007;
   
   
  for(;;) {
      
		c_micro = micros_10();
		
		if(c_micro - p_micro > 2000){  // 20ms마다 진입
			p_micro = c_micro;
			motor_duty++;
			
			if(motor_duty >= 230) motor_duty=70;
		}
		
		c_micro2 = micros_10();
		if(c_micro2 - p_micro2 > 2000){
			p_micro2 = c_micro2;
			motor_duty2 += 4;
			
			if(motor_duty2 >= 230) motor_duty2 = 70;
		}
      
  }
   
}

   

/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/