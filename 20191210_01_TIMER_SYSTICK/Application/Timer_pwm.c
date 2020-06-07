
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization


volatile unsigned long count = 0;
volatile unsigned long count_micros = 0;
volatile unsigned long sys_count = 0;
volatile unsigned int TimingDelay;
 
 volatile unsigned int test_count_pwm = 0, test_var = 0;

int ledLight = 0;
int ledLight1 = 0;
int ledLight2 = 0;
int ledLight3 = 0;
unsigned long timer_count = 0;
int pwm_duty = 0;
int servo_duty = 700;
int count_pwm = 0;
int count_servo = 0;
volatile unsigned int row_num = 0;
int i = 0;
unsigned char reg_data = 0;
unsigned int one[8] = {0x0000, 0x3800, 0x3300, 0x0400, 0x1800, 0x0400, 0x4400, 0x3800};

void Delay(unsigned int nTime);
void TimingDelay_Decrement(void);
unsigned long micros_10us(void);
void static_led_pwm();
void static_servo();


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
void TIM2_IRQHandler (void) { // 

  if ((TIM2->SR & 0x0001) != 0) {                 // check update interrupt source
		
		GPIOA->BSRR = 0x01 << 5;
		
    TIM2->SR &= ~(1<<0);                          // clear UIF flag
 }
	else if ((TIM2->SR & 0x0002) != 0) {                 // check capture/compare interrupt source
		
		GPIOA->BRR = 0x01 << 5;

    TIM2->SR &= ~(1<<1);                          // clear UIF flag
 }

} // end TIM2_UP_IRQHandler





void TIM3_IRQHandler (void) { // 

  if ((TIM3->SR & 0x0001) != 0) {                 // check update interrupt source
		
		 //static_servo();
		GPIOA->BSRR = 0x01 << 8;
		
    TIM3->SR &= ~(1<<0);                          // clear UIF flag
 }
	else if((TIM3->SR & 0x0002) != 0){
		GPIOA->BRR = 0x01 << 8;
		
		TIM3->SR &= ~(1<<1);
	}
	

} // end TIM3_UP_IRQHandler







unsigned long micros_10us(){
	return sys_count;
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
	 sys_count++;
}




void static_servo(){
		count_servo++;
	if(count_servo == 625){  // 20ms   -> 32us 한카운트
		count_servo = 0;
		GPIOA->BSRR = 0x01 << 8;
	}
	else if(count_servo == servo_duty){
		GPIOA->BRR = 0x01 << 8;
	}
}


void static_led_pwm(){  // 100us마다 진입
	count_pwm++;
	if(count_pwm == 100){  // 10ms
		count_pwm = 0;
		GPIOA->BSRR = 0x01 << 5;
	}
	else if(count_pwm == pwm_duty){
		GPIOA->BRR = 0x01 << 5;
	}
	
}




 
 unsigned long p_micros1;
 unsigned long p_micros2;
 unsigned long c_micros;
int led_toggle = 0;

/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  stm32_Init ();                                // STM32 setup
	
	// SysTick
	SysTick->LOAD = 720;  // 10us
	SysTick->CTRL = 0x07;  // enable register
	
	
	// Clock
	RCC->APB2ENR |= 0x01 << 2;  // GPIOA clock enable
	RCC->APB1ENR |= 0x01 << 0;  // timer2 clock enable
	RCC->APB1ENR |= 0x01 << 1;  // timer3 clock enable
	
	
	// GPIOA.5 : GPIO output push/pull 50MHz
	GPIOA->CRL &= ~(0x0f << 20);
	GPIOA->CRL |= 0x03 << 20;
	
	
	// GPIOA.8 : GPIO output push/pull 50MHz
	GPIOA->CRH &= ~(0x0f << 0);
	GPIOA->CRH |= 0x03 << 0;
	
	
	// timer2
	TIM2->CR1 |= 0x01;  // count enable
	TIM2->PSC = 72;  // 1us가 한 주기 
	TIM2->ARR = 2000;  // 2ms마다 인터럽트 발생
	TIM2->CNT = 0x00;  // 한 주기마다 이게 하나씩 늘어남
	TIM2->DIER |= 0x03;  // update, capture/compare interrupt enable
	TIM2->CCMR1 = 0x60;  // Fast-PWM 허용
	TIM2->CCR1 = 200;  // 200us마다 Capture/Compare Interrupt 발생
	
	
	// timer3
	TIM3->CR1 |= 0x01;  // count enable
	TIM3->PSC = 72;  // 1us가 한 주기 
	TIM3->ARR = 20000;  // 20ms마다 인터럽트 발생
	TIM3->CNT = 0x00;  // 한 주기마다 이게 하나씩 늘어남
	TIM3->DIER |= 0x03;  // update, capture/compare interrupt enable
	TIM3->CCMR1 = 0x60;  // Fast-PWM 허용
	TIM3->CCR1 = 2000;  // 2ms마다 Capture/Compare Interrupt 발생


	// NVIC
	NVIC->ISER[0] |= 0x01 << 28;  // timer2 interrupt enable
	NVIC->ISER[0] |= 0x01 << 29;  // timer3 interrupt enable
	
	
	
	
	
	//=============================================================
	
	while(1){
		c_micros = micros_10us();
		if(c_micros - p_micros1 > 1000){  // 10ms 
			
			p_micros1 = c_micros;
			
			pwm_duty += 20;
			if(pwm_duty > 1999) pwm_duty = 20;
			
			TIM2->CCR1 = pwm_duty;
				
		}
		
		
		if(c_micros - p_micros2 > 2000){  // 20ms
			p_micros2  = c_micros;
			
			servo_duty += 16;
			if(servo_duty > 2300) servo_duty = 700;
			TIM3->CCR1 = servo_duty;
			
		}
		
	}
	
	//=============================================================
	
	
	
	
	
	//=============================================================
	
//	while(1){
//		c_micros = micros_10us();
//		if(c_micros - p_micros1 > 1000){  // 10ms 
//			
//			p_micros1 = c_micros;
//			
//			pwm_duty += 20;
//			if(pwm_duty > 1999) pwm_duty = 20;
//			
//			TIM2->CCR1 = pwm_duty;
//				
//		}
//		
//	}
	
	//=============================================================
	
	
	
	
	
		//=============================================================
	
//	while(1){
//		c_micros = micros_10us();
//		if(c_micros - p_micros1 > 1000){  // 10ms 
//			
//			p_micros1 = c_micros;
//			
//			pwm_duty++;
//			if(pwm_duty == 99) pwm_duty = 1;
//				
//		}
//		
//	}
	
	//=============================================================
	
	
	
	
	
	//=============================================================
	
//	while(1){
//	
//		c_micros = micros_10us();
//		if(c_micros - p_micros1 > 10000){  // 100ms 
//			
//			p_micros1 = c_micros;
//			
//			if(led_toggle == 0){
//				led_toggle = 1;
//				GPIOA->BSRR = 0x01 << 5;
//			}
//			else if(led_toggle == 1){
//				led_toggle = 0;
//				GPIOA->BRR = 0x01 << 5;
//			}
//			
//		}
//		
//	}
	
	//=============================================================
	
	
	
	
	
	
	//=============================================================
	
//	while(1){
//	
//		c_micros = micros_10us();
//		if(c_micros - p_micros1 > 10000){  // 100ms 
//			
//			p_micros1 = c_micros;
//			
//			if(led_toggle == 0){
//				led_toggle = 1;
//				GPIOA->BSRR = 0x01 << 5;
//			}
//			else if(led_toggle == 1){
//				led_toggle = 0;
//				GPIOA->BRR = 0x01 << 5;
//			}
//			
//		}
//		
//	}
	
	//=============================================================
	
	
	
	
	
	//=============================================================
	
//	while(1){
//	
//		GPIOA->ODR |= 0x01 << 5;
//		// GPIOA->BSRR = 0x01 << 5;
//		Delay(100);
//		
//		GPIOA->BRR = 0x01 << 5;
//		// GPIOA->ODR &= ~(0x01 << 5);
//		Delay(100);
//		
//	}
	
	//=============================================================


}
		

/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
