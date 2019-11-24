
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization


volatile unsigned long count=0;
 volatile unsigned int TimingDelay;

int ledLight = 0;
int ledLight1 = 0;
int ledLight2 = 0;
int ledLight3 = 0;
unsigned long timer_count = 0;
int pwm_var = 0;
int count_pwm = 0;
volatile unsigned int row_num = 0;
int i = 0, k=0;
unsigned long ii = 0;
unsigned char reg_data = 0;
unsigned int one[8] = {0x0000, 0x3800, 0x3300, 0x0400, 0x1800, 0x0400, 0x4400, 0x3800};

void Delay(unsigned int nTime);
void delay_for(void);
void TimingDelay_Decrement(void);

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
//		GPIOA->ODR |= (0x01 << 5);
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

//		GPIOA->ODR &= ~(0x01 << 5);
    TIM1->SR &= ~(1<<1);                          // clear UIF flag
 }
} // end TIM1_UP_IRQHandler





/*----------------------------------------------------------*\
                        |
\*----------------------------------------------------------*/
void TIM2_IRQHandler (void) {
	int rowrow = 0;

  if ((TIM2->SR & 0x0001) != 0) {                 // check interrupt source
//		count_pwm++;
//		
//	ledLight2 = ~ledLight2;
//	if( ledLight2 )
//    	GPIOA->ODR &= ~(0x01 << 5);                           // switch on LED
//	else
//    	GPIOA->ODR |=   (0x01 << 5);                          // switch off LED

//		GPIOA->ODR &= ~(0x01 << 5);
//		if(count_pwm == 100){
//			count_pwm = 0;
//			GPIOA->ODR |=   (0x01 << 5);
//		}
//		
//		else if(count_pwm == pwm_var){ 
//			GPIOA->ODR &= ~(0x01 << 5); 
		
		//GPIOA->ODR |=   (0x01 << 5);
	//	}
//	ledLight2 = ~ledLight2;
			
			
//	if( ledLight2 )
//    	GPIOA->ODR &= ~(0x01 << 5);                           // switch on LED
//	else
//    	GPIOA->ODR |=   (0x01 << 5);                          // switch off LED

//	GPIOA->ODR |= (0x01 << 5);
//		

//		// -------------------------------
//		// 1. all row off
//		GPIOC->ODR = 0x0000;  // row all off
//		// -------------------------------
//		
//		
//		
//		// -------------------------------
//		// 2. setting one column value
//		for(i=0; i<8; i++){
//			if(!(one[row_num] & (0x8000 >> i))){
//				GPIOB->ODR |= 0x0100 << i;
//			}
//		}
//		// -------------------------------
//		
//		
//		
//		// -------------------------------
//		// 3. one row on
//		GPIOC->ODR = 0x00000001 << row_num;
//		// -------------------------------
//		
//		
//		
//		// -------------------------------
//		// 4. row number change
//		row_num++;
//		if(row_num == 7){
//			row_num = 0;
//		}
//		// -------------------------------
    TIM2->SR &= ~(1<<0);                          // clear UIF flag
 }
	
// if ((TIM2->SR & 0x0002) != 0) {                 // check interrupt source
//GPIOA->ODR &= ~(0x01 << 5); 
	 
	 
	 

//		
//    TIM2->SR &= ~(1<<1);                          // clear UIF flag
// }
} // end TIM2_UP_IRQHandler


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

void delay_for(void){
	for(k=0;k<12;k++);  // 1us delay
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
	
//	RCC->APB2ENR &= ~(0x01 << 11);
//	RCC->APB1ENR = 0x01;
//	RCC->APB2ENR = 0x04;     // PORTA clock enable
//	RCC->APB2ENR |= 0x18;     // PORTC, PORTB clock enable

	RCC->APB2ENR = 0x0000501C;
	
	GPIOA->CRL  =  0xB0BB0B04;
	GPIOC->CRL = 0xB4BB3333;
	
	
	SPI1->CR1 = 0x0B6C;
	SPI1->CR2 = 0x00;
	SPI1->DR = 0x0000;
	
	
	
//	
//	GPIOC->CRL = 0x33333333;
//	GPIOB->CRH = 0x33333333;
//	GPIOC->ODR = 0x0000;  // row all off
//	
//	TIM2->CR1 = 0x01;
//	TIM2->DIER = 0x01;
//	TIM2->CNT = 0;
//	TIM2->PSC = 720;
//	TIM2->ARR = 250;
//	
//	NVIC->ICER[0] |= (0x01 << 25);     // timer1 clear
//	NVIC->ISER[0] |= (0x01 << 28);     // timer2 update interrupt set
	
	
//	
//	GPIOB->ODR |= 0x20;
	

	
	// -------------------------------------------------------------------
	// Systick enable
	//SysTick->LOAD = 720;     // 10us interrupt
	//SysTick->CTRL = 0x07;     // enable
	// -------------------------------------------------------------------
	
	
 
//	*(volatile unsigned int *)0x4002101C = 0x01; // RCC에서 APB1ENR에 TIM2 활성화
//	*(volatile unsigned int *)0xE000EF00 = 0x10000000; // NVIC에서 TIMER2 세팅
//	*(volatile unsigned int *)0x40000000 = 0x01; // TIM2 CEN 활성화
//	*(volatile unsigned int *)0x4000000C = 0x01; // TIM2 UIE 활성화
//	*(volatile unsigned int *)0x40000024 = 0x00; // TIM2의 CNT를 0으로
//	*(volatile unsigned int *)0x40000028 = 7200; // TIM2의 PSC에 분주비 설정
//	*(volatile unsigned int *)0x4000002C = 10; // TIM2의 ARR에 1000 설정
	
	
	for(;;) {
				
		SPI1->DR = 0xaaaa;     // 상위 2비트는 col, 하위 2비트는 row
		
		GPIOC->ODR = 0xff;
		delay_for();
		GPIOC->ODR = 0x00;
		
		for(ii=0;ii<300000;ii++){
			delay_for();
		}
		
		SPI1->DR = 0x5555;     // 상위 2비트는 col, 하위 2비트는 row
		
		GPIOC->ODR = 0xff;
		delay_for();
		GPIOC->ODR = 0x00;
		
		for(ii=0;ii<300000;ii++){
			delay_for();
		}
		
//	c_micros = micros_10us();
//		
//	if(c_micros - p_micros > 250){
//		
//		p_micros = c_micros;
//		
//		// -------------------------------
//		// 1. all row off
//		GPIOC->ODR = 0x0000;  // row all off
//		// -------------------------------
//		
//		
//		
//		// -------------------------------
//		// 2. setting one column value
//		for(i=0; i<8; i++){
//			if(!(one[row_num] & (0x8000 >> i))){
//				GPIOB->ODR |= 0x0100 << i;
//			}
//		}
//		// -------------------------------
//		
//		
//		
//		// -------------------------------
//		// 3. one row on
//		GPIOC->ODR = 0x00000001 << row_num;
//		// -------------------------------
//		
//		
//		
//		// -------------------------------
//		// 4. row number change
//		row_num++;
//		if(row_num > 7){
//			row_num = 0;
//		}
//		// -------------------------------
//	}
		
		//GPIOA->ODR &= ~0x20;
		
//		ledLight1 = ~ledLight1;
//		if( ledLight1 )
//			GPIOA->ODR &= ~0x20;                           // switch on LED
//		else
//			GPIOA->ODR |= 0x20; 
//		
//		Delay(1000);
	
//		c_micros = micros_10us();
//		if(c_micros - p_micros > 10){ // 100 ms마다 진입
//			p_micros = c_micros;
//			
//			count_pwm++;
//			
//			if(count_pwm == 100){
//				count_pwm = 0;
//				GPIOA->ODR |= 0x20;
//			}
//			else if(count_pwm == pwm_duty){
//				GPIOA->ODR &= ~(0x20);
//			}
			//TIM2->CCR1 = pwm_var;
				
//			pwm_var += 10;
//			if(pwm_var > 900) pwm_var = 0;
//				TIM2->CCR1 = pwm_var;
		}
		
		
//		
//		if(c_micros - p_micros1 > 1000){ // 10ms마다 진입
//		
//		p_micros1 = c_micros;
//		
//		pwm_duty++;
//		if(pwm_duty == 99) pwm_duty = 0;
//		}
	}

/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
