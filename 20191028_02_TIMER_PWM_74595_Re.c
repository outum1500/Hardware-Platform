
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization


volatile unsigned long count=0;
volatile unsigned int TimingDelay;

int flag = 0;
int ledLight = 0;
int ledLight1 = 0;
int ledLight2 = 0;
int ledLight3 = 0;
int value_74595 = 0;
unsigned long timer_count = 0;
int pwm_var = 0;
int count_pwm = 0;
volatile unsigned int row_num = 0;
int i = 0;
unsigned char reg_data = 0;
unsigned int one[8] = {0x0000, 0x3800, 0x4400, 0x0400, 0x1800, 0x0400, 0x4400, 0x3800};

void Delay(unsigned int nTime);
void TimingDelay_Decrement(void);
void set_74595(unsigned short value);

void delay_for(){
	int i;
	for(i=0;i<72;i++){  //1000us ¿¹»ó
	}
}

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
  	GPIOA->ODR |= (0x01 << 5);
    TIM1->SR &= ~(1<<0);                          // clear UIF flag
 }
} // end TIM1_UP_IRQHandler

void TIM1_CC_IRQHandler (void) {

  if ((TIM1->SR & 0x0002) != 0) {                 // check interrupt source
		GPIOA->ODR &= ~(0x01 << 5);
    TIM1->SR &= ~(1<<1);                          // clear UIF flag
 }
} // end TIM1_UP_IRQHandler



/*----------------------------------------------------------*\
                        |
\*----------------------------------------------------------*/
void TIM2_IRQHandler (void) {

  if ((TIM2->SR & 0x0001) != 0) {                 // check interrupt source
	
		
		value_74595 = 0x0000;
		set_74595(value_74595);
		
		value_74595 = one[row_num];
		set_74595(value_74595);
		
		value_74595 |= 0x0001 << row_num;
		set_74595(value_74595);

		
		// -------------------------------
		// 4. row number change
		row_num++;
		if(row_num > 7){
			row_num = 0;
		}
		// -------------------------------
		
    TIM2->SR &= ~(1<<0);                          // clear UIF flag
		

 }
	
// if ((TIM2->SR & 0x0002) != 0) {                 // check interrupt source
//GPIOA->ODR &= ~(0x01 << 5); 
	 
	 
	 

//		
//    TIM2->SR &= ~(1<<1);                          // clear UIF flag
// }
} // end TIM2_UP_IRQHandler

void set_74595(unsigned short value){
	for(i=0;i<16;i++){
		if(i<=7){
				if(!(value & (0x0100 << i))){
					GPIOC->ODR |= 0x01;
				}
				else{
					GPIOC->ODR &= ~0x01;
				}
			}
		else if(i>7){
			if((value & (0x8000 >> i))){
				GPIOC->ODR |= 0x01;
			}
			else{
				GPIOC->ODR &= ~0x01;
			}
		}
				
				GPIOC->ODR |= 0x02;
				delay_for();
				GPIOC->ODR &= ~0x02;
				delay_for();
			}
			GPIOC->ODR |= 0x04;
			delay_for();
			GPIOC->ODR &= ~0x04;
			delay_for();
		
}
unsigned long micros_10us(){
	return count;
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
	 //TimingDelay_Decrement();
	//count++;
	 
//	 if(ledLight1 == !ledLight1){
//	 
//	 }
	
	 
//	 #if 0
//	ledLight1 = ~ledLight1;
//	if( ledLight1 )
//    	GPIOB->ODR &= ~LED1;                           // switch on LED
//	else
//    	GPIOB->ODR |=  LED1; 				
//		
//	if(count++ == 1000){
//		count=0;
//		ledLight3 = ~ledLight3;
//		if( ledLight3 )
//	    	GPIOB->ODR &= ~(0x01 << 4);                           // switch on LED
//		else
//	    	GPIOB->ODR |=  (0x01 << 4); 
//	}
//			#endif
//		                          // switch off LED
}
 
 unsigned long p_micros;
 unsigned long p_micros1;
 unsigned long c_micros;
unsigned long pwm_duty = 0;

/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  stm32_Init ();                                // STM32 setup
	
	RCC->APB2ENR |= 0x14;     // PORTA, PORTC clock enable
	RCC->APB2ENR &= ~0x00000800;
	RCC->APB1ENR = 0x01;
	GPIOC->CRL = 0x33333333;     // PORTC 50MHz output push-pull setting
	GPIOA->CRL = 0x33333333;
	GPIOC->ODR = 0x00000000;     // all low
	
	
	
//	RCC->APB2ENR &= ~(0x01 << 11);
//	RCC->APB1ENR = 0x01;

	
//	RCC->APB2ENR |= 0x18;     // PORTC, PORTB clock enable
//	
//	
//	GPIOC->CRL = 0x33333333;
//	GPIOB->CRH = 0x33333333;
//	GPIOC->ODR = 0x0000;  // row all off
//	
	TIM2->CR1 = 0x01;
	TIM2->DIER = 0x01;
	TIM2->CNT = 0;
	TIM2->PSC = 720;
	TIM2->ARR = 250;
	
	NVIC->ICER[0] |= (0x01 << 25);     // timer1 clear
	NVIC->ISER[0] |= (0x01 << 28);     // timer2 update interrupt set
	
	
	for(;;) {
		

	}
		
		
	}

/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
