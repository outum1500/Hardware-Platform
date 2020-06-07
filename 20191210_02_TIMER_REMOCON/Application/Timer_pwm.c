
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



/* remocon */
unsigned long falling_time = 0;
unsigned long rising_time = 0;
unsigned long diff_time = 0;
unsigned long remocon_time[40];
unsigned long remocon_diff_time[20];
int rem_count = 0;
int remocon_flag = 0;
int remocon_hex_data;
int remocon_data_bit[40];
char remocon_data = 0;
char rem_data[10] = {0x68, 0x30, 0x18, 0x7A, 0x10, 0x38, 0x5A, 0x42, 0x4a, 0x52};
int remocon_value = 0;
int remocon_pre_value = 0;



/* adc */
unsigned short adc_value_x = 0;



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

void EXTI0_IRQHandler(void){
	
	if(EXTI->PR & (1<<0)){  // EXTI0 interrupt pending?
	
		remocon_time[rem_count] = micros_10us() * 10;
		
		if(rem_count > 0){
			remocon_diff_time[rem_count-1] = remocon_time[rem_count] - remocon_time[rem_count-1];
			
			if(remocon_diff_time[rem_count-1] > 13000 && remocon_diff_time[rem_count-1] < 14000){
				rem_count = 1;
			}
			if(remocon_diff_time[rem_count-1] > 11000 && remocon_diff_time[rem_count-1] < 12000){
				rem_count = -1;
				remocon_value = remocon_pre_value;
			}
		}

		
		
		rem_count++;
		if(rem_count == 34){
			
			rem_count = 0;
		}
		
		EXTI->PR = (1<<0);  // clear pending interrupt
	}
	
}


void EXTI15_10_IRQHandler(void){
	
	if(EXTI->PR & (1<<13)){  // EXTI0 interrupt pending?
	
		if(GPIOC->IDR & (0x01 << 13)){
			rising_time = micros_10us();
		}
		else{
			falling_time = micros_10us();
			diff_time = (falling_time - rising_time) * 10;
		}
		
		EXTI->PR = (1<<13);  // clear pending interrupt
	}
	
}








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
	if(count_servo == 625){  // 20ms   -> 32us ��ī��Ʈ
		count_servo = 0;
		GPIOA->BSRR = 0x01 << 8;
	}
	else if(count_servo == servo_duty){
		GPIOA->BRR = 0x01 << 8;
	}
}


void static_led_pwm(){  // 100us���� ����
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
	RCC->APB2ENR |= 0x01 << 4;  // GPIOC clock enable
	RCC->APB2ENR |= 0x01 << 9;  // ADC1 clock enable
	RCC->APB1ENR |= 0x01 << 0;  // timer2 clock enable
	RCC->APB1ENR |= 0x01 << 1;  // timer3 clock enable
	RCC->APB2ENR |= 0x01 << 0;  // AFIO clock enable
	RCC->AHBENR |= 0x01 << 0;  // DMA1 clock enable
	
	
	
	// GPIOA.5 : GPIO output push/pull 50MHz
	GPIOA->CRL &= ~(0x0f << 20);
	GPIOA->CRL |= 0x03 << 20;
	
	// GPIOA.8 : GPIO output push/pull 50MHz
	GPIOA->CRH &= ~(0x0f << 0);
	GPIOA->CRH |= 0x03 << 0;
	
	// GPIOC.0 : GPIO floating Input mode
	GPIOC->CRL &= ~(0x0f << 0);
	GPIOC->CRL |= 0x04 << 0;
	
	// GPIOA.0 : GPIO Analog Input mode
	GPIOA->CRL &= ~(0x0f << 0);
	GPIOA->CRL |= 0x04 << 0;
	
	
	
	// EXTI0
	EXTI->IMR |= 0x01;
	EXTI->FTSR |= 0x01;
	AFIO->EXTICR[0] |= 0x02;  // PC.0 Interrupt

	// EXTI13
	EXTI->IMR |= 0x01 << 13;
	EXTI->FTSR |= 0x01 << 13;
	EXTI->RTSR |= 0x01 << 13;
	AFIO->EXTICR[3] |= 0x20;  // PC.13 interrupt
	
	
	
	// timer2
	TIM2->CR1 |= 0x01;  // count enable
	TIM2->PSC = 72;  // 1us�� �� �ֱ� 
	TIM2->ARR = 2000;  // 2ms���� ���ͷ�Ʈ �߻�
	TIM2->CNT = 0x00;  // �� �ֱ⸶�� �̰� �ϳ��� �þ
	TIM2->DIER |= 0x03;  // update, capture/compare interrupt enable
	TIM2->CCMR1 = 0x60;  // Fast-PWM ���
	TIM2->CCR1 = 200;  // 200us���� Capture/Compare Interrupt �߻�
	
	// timer3
	TIM3->CR1 |= 0x01;  // count enable
	TIM3->PSC = 72;  // 1us�� �� �ֱ� 
	TIM3->ARR = 20000;  // 20ms���� ���ͷ�Ʈ �߻�
	TIM3->CNT = 0x00;  // �� �ֱ⸶�� �̰� �ϳ��� �þ
	TIM3->DIER |= 0x03;  // update, capture/compare interrupt enable
	TIM3->CCMR1 = 0x60;  // Fast-PWM ���
	TIM3->CCR1 = 2000;  // 2ms���� Capture/Compare Interrupt �߻�
	
	
	
	
	// DMA
	DMA1_Channel1->CNDTR = 0x01;
	DMA1_Channel1->CPAR = 0x4001244c;  // adc data register
	DMA1_Channel1->CMAR = (unsigned int)&adc_value_x;
	ADC1->CR1 = 0x100;
	ADC1->CR2  =0x1E0103;
	ADC1->SMPR1 = 0x00;
	ADC1->SMPR2 = 0x05;
	DMA1_Channel1->CCR = 0x0521;
	
	
	



	// NVIC
	NVIC->ISER[0] |= 0x01 << 28;  // timer2 interrupt enable
	NVIC->ISER[0] |= 0x01 << 29;  // timer3 interrupt enable
	
	NVIC->ISER[0] |= 0x01 << 6;  // EXTI0 interrupt enable
	NVIC->ISER[1] |= 0x01 << 8;  // EXTI15_10 interrupt enable
	
	
	
	
	
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
		
		
		
		
		if(remocon_flag == 1){
			remocon_flag = 0;
			
			for(i=0;i<32;i++){
				if( remocon_diff_time[i+1] > 1000 && remocon_diff_time[i+1] < 1500 ){
					remocon_data_bit[i] = 0;
				}
				else if( remocon_diff_time[i+1] > 2000 && remocon_diff_time[i+1] < 2500 ){
					remocon_data_bit[i] = 1;
				}
			}
			
			
			remocon_hex_data = 0;
			
			for(i=0;i<8;i++){
				remocon_hex_data = remocon_hex_data << 1;
				if(remocon_data_bit[16+i] == 1){
					remocon_hex_data |= 0x01;
				}
			}
			
			remocon_data = (char)remocon_hex_data;
			
			
			remocon_value = 0;
			
			for(i=0;i<10;i++){
				if(remocon_data == rem_data[i]){
					remocon_value = i;
					remocon_pre_value = i;
					break;
				}
			}
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
