
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization

int ledLight = 0;
int ledLight1 = 0;
volatile unsigned long count=0;
unsigned int TimingDelay;
unsigned long hightime = 0;
unsigned long lowtime = 0;
unsigned long howlong = 0;


int ledLight2 = 0;
int ledLight3 = 0;
int Alarm = 0;
unsigned long buzzerflag = 0;
void Delay(unsigned int nTime);
unsigned long micros_10us();
#define LED             ( 1 << 6 )                // PB5: LED D2
#define LED1             ( 1 << 13 )                // PB5: LED D2



#define __WFI                             __wfi

#define  PWR_CR_PDDS                         ((unsigned short)0x0002)     /*!< Power Down Deepsleep */
#define  PWR_CR_CWUF                         ((unsigned short)0x0004)     /*!< Clear Wakeup Flag */
#define  SCB_SCR_SLEEPDEEP                   ((unsigned char)0x04)               /*!< Sleep deep bit */

void PWR_EnterSTANDBYMode(void)
{
  /* Clear Wake-up flag */
  PWR->CR |= PWR_CR_CWUF;
  /* Select STANDBY mode */
  PWR->CR |= PWR_CR_PDDS;
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR |= SCB_SCR_SLEEPDEEP;
/* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM   )
  __force_stores();
#endif
  /* Request Wait For Interrupt */
  __WFI();
}

/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
//#define LED             ( 1 << 5 )                // PB5: LED D2
#define LED             ( 1 << 6 )                // PB5: LED D2
/*----------------------------------------------------------*\
 | EXTI0 Interrupt Handler                                  |
\*----------------------------------------------------------*/
void EXTI0_IRQHandler(void)
{
	
  if (EXTI->PR & (1<<0)) {                        // EXTI0 interrupt pending?
			int flag = GPIOA->IDR & 0x00000001;
			
			if(flag == 1){    // ECHO 핀이 HIGH이면?
				//GPIOC->ODR = 0x00000001;
				hightime = micros_10us();

			}		
			else if(flag == 0){
				//GPIOC->ODR = 0x00000000;
				lowtime = micros_10us();
				howlong = lowtime - hightime;
			}
		
    if ((ledLight ^=1) == 0) 
    	GPIOA->ODR &= ~(0x01 << 5);                           // switch on LED
    else
    	GPIOA->ODR |=  (0x01 << 5);                           // switch off LED
		

    EXTI->PR = (1<<0);                           // clear pending interrupt
  }
}

/*----------------------------------------------------------*\
 | EXTI1 Interrupt Handler                                  |
\*----------------------------------------------------------*/
void EXTI1_IRQHandler(void)
{
  if (EXTI->PR & (1<<1)) {                        // EXTI1 interrupt pending?
    if ((ledLight1 ^=1) == 0) 
    	GPIOA->ODR &= ~(0x01 << 8);                           // switch on LED
    else
    	GPIOA->ODR |=  (0x01 << 8);                           // switch off LED

    EXTI->PR = (1<<1);                           // clear pending interrupt
  }

	   count = 1;
}



/*----------------------------------------------------------*\
 | EXTI15..10 Interrupt Handler                             |
\*----------------------------------------------------------*/
void EXTI9_5_IRQHandler(void)
{

	
  if (EXTI->PR & (1<<5)) {                       // EXTI0 interrupt pending?
    if ((ledLight ^=1) == 0) 
    	GPIOC->ODR &= ~(0x01 << 8);                           // switch on LED
    else
    	GPIOC->ODR |=  (0x01 << 8);                           // switch off LED

    EXTI->PR = (1<<5);                          // clear pending interrupt
  }
	
	 else if (EXTI->PR & (1<<6)) {                       // EXTI0 interrupt pending?
    if ((ledLight1 ^=1) == 0) 
    	GPIOB->ODR &= ~(0x01 << 7);                           // switch on LED
    else
    	GPIOB->ODR |=  (0x01 << 7);                           // switch off LED

    EXTI->PR = (1<<6);                          // clear pending interrupt
  }
}


/*----------------------------------------------------------*\
 | EXTI15..10 Interrupt Handler                             |
\*----------------------------------------------------------*/
void EXTI15_10_IRQHandler(void)
{
  if (EXTI->PR & (1<<13)) {                       // EXTI0 interrupt pending?
    if ((ledLight ^=1) == 0) 
    	GPIOA->ODR &= ~(0x01 << 5);                           // switch on LED
    else
    	GPIOA->ODR |=  (0x01 << 5);                           // switch off LED

    EXTI->PR = (1<<13);                          // clear pending interrupt
  }
}

void TIM2_IRQHandler (void) {   // 100us 마다 접속

  if ((TIM2->SR & 0x0001) != 0) {                 // check interrupt source
		
		if(howlong < 700){
			GPIOB->ODR = ((buzzerflag) << 5);
			buzzerflag = !buzzerflag;
		}
		
		
	ledLight2 = ~ledLight2;
	if( ledLight2 )
    	GPIOB->ODR &= ~LED1;                           // switch on LED
	else
    	GPIOB->ODR |=  LED1;                           // switch off LED

    TIM2->SR &= ~(1<<0);                          // clear UIF flag
 }
} // end TIM2_UP_IRQHandler

	void Delay(unsigned int nTime)
{
	TimingDelay = nTime;
	
  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void){
	if(TimingDelay != 0x00){
    TimingDelay--;
	}
}

void SysTick_Handler(void){
	TimingDelay_Decrement();
	
	count++;
}

	unsigned long micros_10us()
{
	
	return count;
}

	

/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
	int microflag = 0;
	int a;
	unsigned long c_micro=0;
	unsigned long p_micro=0;
	unsigned long p_micro1=0;
	

  stm32_Init ();                                // STM32 setup
	
	SysTick->CTRL = 0x00000006;
	SysTick->LOAD = 72;
	SysTick->CTRL = 0x00000007;  // 10us interrupt enable
	
	GPIOA->CRH = 0x00000300;
	GPIOC->CRL = 0x00000003;
	GPIOC->CRH = 0x00300000;
//	
	EXTI->IMR = 0x00002001;
	EXTI->FTSR = 0x00002001;
	EXTI->RTSR = 0x00000001;
	AFIO->EXTICR[3] = 0x00000020;;
	NVIC->ISER[0] = 0x00800000;
	
	TIM2->CR1 = 0x01;
	TIM2->DIER = 0x01;
	TIM2->CNT = 0;
	TIM2->PSC = 72;
	TIM2->ARR = 1275;
	
	NVIC->ISER[0] |= (0x01 << 28);     // timer2 update interrupt set
	
	
	while(1){
		
		
		GPIOA->ODR = 0x00000400;
		Delay(10);
		GPIOA->ODR = 0x00000000;
		Delay(10); // 10us
		Delay(100000); // 100ms
		
//		TIM2->CR1 = 0x0001;
//		Delay(1000000);
//		TIM2->CR1 = 0x0000;
//		Delay(1000000);
		
		
		c_micro = micros_10us();
		
		if(howlong < 700 && howlong > 500){
			if(c_micro - p_micro > 300000){  //300ms만에 들어감
				
				p_micro = c_micro;
	
				if(microflag == 1){
					GPIOC->ODR = 0x00000001;
					TIM2->CR1 = 0x0001;
					microflag = 0;
				}
				else if(microflag == 0){
					GPIOC->ODR = 0x00000000;
					TIM2->CR1 = 0x0000; // timer2 off
					microflag = 1;
				}
			}
		}
		else if(howlong < 500 && howlong > 300){
			if(c_micro - p_micro > 100000){  //100ms만에 들어감
				
				p_micro = c_micro;
	
				if(microflag == 1){
					GPIOC->ODR = 0x00000001;
					TIM2->CR1 = 0x0001;
					microflag = 0;
				}
				else if(microflag == 0){
					GPIOC->ODR = 0x00000000;
					TIM2->CR1 = 0x0000; // timer2 off
					microflag = 1;
				}
			}
		}
		else if(howlong < 300){
			TIM2->CR1 = 0x0001;
		}
		
	}
	
	
	/////////////////////////INPUT : PA1, OUTPUT : PA8////////////////////////////////
//	*(volatile unsigned int *)(0x40021018) |= 0x04; // APB2ENR
//	*(volatile unsigned int *)(0x40010800) = 0x40; // GPIOA->CRL   1번
//	*(volatile unsigned int *)(0x40010804) = 0x03; // GPIOA->CRH   8번
//	
//	*(volatile unsigned int *)(0x4001040c) = 0x02; // EXTI->FTSR
//	*(volatile unsigned int *)(0x40010400) = 0x02; // EXTI->IMR
//	
//	*(volatile unsigned int *)(0xE000E100) |= 0x80; // NVIC->ISER0
	/////////////////////////////////////////////////////////////////////////////////////
	
//	*(volatile unsigned int *)(0x40021018) |= 0x1C; // APB2ENR
//	*(volatile unsigned int *)(0x40010800) = (0x04 << 20); // GPIOA.5->CRL
//	//*(volatile unsigned int *)(0x40010800) |= (0x04 << 20); 
//	*(volatile unsigned int *)(0x40011004) = 0x03; // GPIOC.8->CRH
//	
//	*(volatile unsigned int *)(0x4001040c) = 0x20; // EXTI->FTSR
//	*(volatile unsigned int *)(0x40010400) = 0x20; // EXTI->IMR
//	
//	*(volatile unsigned int *)(0xE000E100) |= 0x00800000; // NVIC->ISER0.23
	
	
	
/******************************************************************************/
//	*(volatile unsigned int *)(0x40021018) |= 0x1C; // APB2ENR
//	*(volatile unsigned int *)(0x40010800) = 0x04400000; // GPIOA.5->CRL, 6번도
//	//*(volatile unsigned int *)(0x40010800) = (0x04 << 24); // GPIOA.6->CRL
//	//*(volatile unsigned int *)(0x40010800) |= (0x04 << 20); 
//	*(volatile unsigned int *)(0x40011004) = 0x03; // GPIOC.8->CRH
//	*(volatile unsigned int *)(0x40010c00) = (0x03 << 28); // GPIOB.7->CRH
//	
//	*(volatile unsigned int *)(0x4001040c) = 0x60; // EXTI->FTSR
//	*(volatile unsigned int *)(0x40010400) = 0x60; // EXTI->IMR
//	
//	*(volatile unsigned int *)(0xE000E100) |= 0x00800000; // NVIC->ISER0.23
	/*****************************************************************************/
	
	
  *(volatile unsigned int *)(0x40021018) |= 0x09; // APB2ENR
	*(volatile unsigned int *)(0x4001000c) = 0x0100; // AFIO에서 EXTI6번만 enable
	*(volatile unsigned int *)(0x40010c00) = 0x34000000; // GPIOB.5는 Input, 6은 Output
	*(volatile unsigned int *)(0x40010800) = (0x04 << 24); // GPIOA.6->CRL
	*(volatile unsigned int *)(0x40011004) = 0x03; // GPIOC.8->CRH
	
  *(volatile unsigned int *)(0x4001040c) = 0x60; // EXTI->FTSR
  *(volatile unsigned int *)(0x40010400) = 0x60; // EXTI->IMR
	
  *(volatile unsigned int *)(0xE000E100) |= 0x00800000; // NVIC->ISER0.23
	
	
	
// Interrupt Group Priority
//SCB->AIRCR |=0x05FA0300; 
// =>  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup; 

//NVIC->IPR[1] |=0x00;

// Interrupt enable
//NVIC->ISER[0]=0x00;

//Port enable
//RCC->APB2ENR |= 0x00;

// Port Select
//AFIO->EXTICR[0]=0x00;

// External Interrupt
//EXTI->IMR = 0x00;
//EXTI->RTSR      |= 0x00;
//EXTI->FTSR      |= 0x00;


	for(;;) {
		
//		a= GPIOA->IDR;
//		
//		
//		if(a& 0x01)
//			 GPIOA->ODR |= (0x01 << 8); 
//		else
//			GPIOA->ODR &= ~(0x01 << 8); 
//		
//		
//		
//#if 0 
//      GPIOB->ODR &= ~(0x01 << 5);                           // switch on LED
//    GPIOB->ODR &= ~(0x01 << 8);                          // switch on LED
//    GPIOB->ODR |= (0x01 << 9);                         // switch on LED
//	Delay(2000000);
//    GPIOB->ODR |=  (0x01 << 5);                           // switch off LED
//    GPIOB->ODR |=  (0x01 << 8);                           // switch on LED
//    GPIOB->ODR &=  ~(0x01 << 9);                            // switch on LED
//	Delay(2000000);

//	if(count == 1){
//		count = 0;
//  		//__WFI();  // Sleep Mode Test
//		PWR_EnterSTANDBYMode();	  // Standby Mode Test
//	 }

//#endif
  	}
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
