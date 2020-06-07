
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization


unsigned int count=0;

int ledLight = 0;
int ledLight1 = 0;
int ledLight2 = 0;
int ledLight3 = 0;
int timer_count = 0;
int pwm_var = 0;
int count_pwm = 0;
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
//#define LED             ( 1 << 5 )                // PB5: LED D2
#define LED             ( 1 << 6 )                // PB5: LED D2
#define LED1             ( 1 << 7 )                // PB5: LED D2
#define LED2             ( 1 << 8 )                // PB5: LED D2
/*----------------------------------------------------------*\
 | Timer1 Update Interrupt Handler                          |
\*----------------------------------------------------------*/
void TIM1_UP_IRQHandler (void) {

  if ((TIM1->SR & 0x0001) != 0) {                 // check interrupt source

//	ledLight = ~ledLight;
//	if( ledLight )
//    	GPIOC->ODR &= ~(0x01 << 13);                           // switch on LED
//	else
//    	GPIOC->ODR |=  0x01 << 13;                           // switch off LED

//		
//		GPIOC->ODR |= 0x01 << 13; 
//		
		GPIOA->ODR |= (0x01 << 5);
    TIM1->SR &= ~(1<<0);                          // clear UIF flag
		

 }
} // end TIM1_UP_IRQHandler


void TIM1_CC_IRQHandler (void) {

  if ((TIM1->SR & 0x0002) != 0) {                 // check interrupt source

//	ledLight = ~ledLight;
//	if( ledLight )
    	GPIOC->ODR &= ~(0x01 << 13);                           // switch on LED
//	else
//    	GPIOB->ODR |=  LED;                           // switch off LED

		GPIOA->ODR &= ~(0x01 << 5);
    TIM1->SR &= ~(1<<1);                          // clear UIF flag
 }
} // end TIM1_UP_IRQHandler





/*----------------------------------------------------------*\
                        |
\*----------------------------------------------------------*/
void TIM2_IRQHandler (void) {

	if ((TIM2->SR & 0x0001) != 0) {                 // check interrupt source
		count_pwm++;
		
		if(count_pwm == 100){
			count_pwm = 0;
			GPIOA->ODR |=   (0x01 << 5);
		}
		
		else if(count_pwm == pwm_var){ 
			GPIOA->ODR &= ~(0x01 << 5); 
		
	TIM2->SR &= ~(1<<0);                          // clear UIF flag
	
}
} // end TIM2_UP_IRQHandler


unsigned long micros_10us(){
	return count;
}

void SysTick_Handler(void)
 {
	 count++;

 }
 
 unsigned long p_micros;
 unsigned long p_micros1;
 unsigned long c_micros;

/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  stm32_Init ();                                // STM32 setup
	
	
	for(;;) {
		
		c_micros = micros_10us();
		
		if(c_micros - p_micros > 1000){
			
			p_micros = c_micros;
			pwm_var++;
			
			if(pwm_var == 100){
				pwm_var = 0;
			}
			
		}
	}
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
