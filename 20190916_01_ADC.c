
#include <stdio.h>

#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 5 )              // PB5: LED D2
/*----------------------------------------------------------*\
 | SOFTWARE DATA                                            |
\*----------------------------------------------------------*/


unsigned short  analog [3];


/*----------------------------------------------------------*\
 | SendChar                                                 |
 | Write character to Serial Port.                          |
\*----------------------------------------------------------*/
int SendChar (int ch)  {

  while (!(USART1->SR & USART_FLAG_TXE));
  USART1->DR = (ch & 0x1FF);

  return (ch);
}
/*----------------------------------------------------------*\
 | GetKey                                                   |
 | Read character to Serial Port.                           |
\*----------------------------------------------------------*/
int GetKey (void)  {

  while (!(USART1->SR & USART_FLAG_RXNE));

  return ((int)(USART1->DR & 0x1FF));
}


void USART1_IRQHandler (void) {
  volatile unsigned int IIR;
  char receive_char;

    IIR = USART1->SR;
    if (IIR & USART_FLAG_RXNE) {                  // read interrupt
      USART1->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
	receive_char =  (USART1->DR & 0x1FF);
//	printf("received data is %c..\n\r",receive_char);
	printf("uart 1 : %c",receive_char);

    }
 /*
    if (IIR & USART_FLAG_TXE) {
      USART1->SR &= ~USART_FLAG_TXE;	          // clear interrupt
	//USART1->DR = '5';
	printf("sent data is %c..\n\r",USART1->DR);
    }
 */
}


void USART2_IRQHandler (void) {
  volatile unsigned int IIR;
  char receive_char;

    IIR = USART2->SR;
    if (IIR & USART_FLAG_RXNE) {                  // read interrupt
      USART2->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
	receive_char =  (USART2->DR & 0x1FF);
//	printf("received data is %c..\n\r",receive_char);
	printf("uart2 : %c",receive_char);

    }
}


void EXTI0_IRQHandler(void)
{
  if (EXTI->PR & (1<<0)) {                        
	printf("in exti0 irq handler...\r\n");
    EXTI->PR |= (1<<0);                           // clear pending interrupt
  }
}

void EXTI1_IRQHandler(void)
{
  if (EXTI->PR & (1<<1)) {                        
	printf("in exti0 irq handler...\r\n");
	PWR_EnterSTANDBYMode();

    EXTI->PR |= (1<<1);                           // clear pending interrupt
  }
}


void adc_Init (void) {

//  GPIOA->CRL &= ~0x0000000F;                   /* set PIN1 analog input (see stm32_Init.c) */

  RCC->AHBENR |= (1<<0);                       /* enable periperal clock for DMA       */

//  DMA1_Channel1->CMAR  = (unsigned long)&analog; /* set chn1 memory address     */
  DMA1_Channel1->CMAR  = (unsigned long)analog; /* set chn1 memory address     */
  DMA1_Channel1->CPAR  = (unsigned long)&(ADC1->DR);     /* set chn1 peripheral address */
  DMA1_Channel1->CNDTR = 3;                     /* transmit 3 words                     */
//  DMA_Channel1->CCR   = 0x00002520;            /* configure DMA channel 1              */
  DMA1_Channel1->CCR   = 0x000025A0;            /* configure DMA channel 1              */
                                               /* circular mode, memory increment mode */
                                               /* memory & peripheral size 16bit       */
                                               /* channel priotity high                */
  DMA1_Channel1->CCR  |= (1 << 0);              /* enable DMA Channe1                   */

  RCC->APB2ENR |= (1<<9);                      /* enable periperal clock for ADC1      */

  ADC1->SQR1  = 0x00200000;                    /* three conversions                    */
  ADC1->SQR3  = (3<<10) | (2<<5) | (1<<0);     /* set order to chn1 - chn2 - chn3      */
  ADC1->SMPR2 = (5<< 9) | (5<<6) | (5<<3);     /* set sample time (55,5 cycles)        */ 

  ADC1->CR1   =  0x00000100;                   /* use independant mode, SCAN mode      */
  ADC1->CR2   =  0x000E0103;                   /* data align right, cont. conversion   */
                                               /* EXTSEL = SWSTART                     */ 
                                               /* enable ADC, DMA mode                 */
  ADC1->CR2  |=  0x00500000;                   /* start SW conversion                  */
  }


void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
  }
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  	char s[20];
	int i;
	
	stm32_Init ();                                // STM32 setup

//adc_Init();
	
	/*
	
	*(volatile unsigned int *)0x40021018 = 0x00000200;  // RCC에서 APB2ENR에서 ADC1EN 활성화
	*(volatile unsigned int *)0x40021014 = 0x15;  // RCC에서 AHBENR에서 DM1AEN 활성화
	*(volatile unsigned int *)0x40020008 = 0x000005A0;  // DMA에서 MSIZE, PSIZE 16bit로 설정, DMA에서 CIRC, MINC 설정
	*(volatile unsigned int *)0x4002000C = 0x03; // DNA에서 CNDTR1 카운트 값을 3으로 설정
	*(volatile unsigned int *)0x40020010 = 0x4001244C; // DMA에서 CPAR 값 설정
	*(volatile unsigned int *)0x40020014 = 0x20002000; // DNA에서 CMAR 값 설정
	
	*(volatile unsigned int *)0x40020008 = 0x000005A1;  // DMA에서 enable 설정
	
	*(volatile unsigned int *)0x4001242C = 0x00200000; // ADC1에서 SQR1에서 레벨3으로 설정
	*(volatile unsigned int *)0x40012434 = 0x00000820; // ADC1에서 SQR3에서 시퀀스별 채널 설정(0-1-2)
	*(volatile unsigned int *)0x40012410 = 0x0000016D; // ADC1에서 SMPR2에서 채널 0~2까지 사이클 55.5로 설정
	
	*(volatile unsigned int *)0x40012404 = 0x00000100; // ADC1에서 CR1에서 SCAN 활성화
	*(volatile unsigned int *)0x40012408 = 0x001E0103; // ADC1에서 CR2에서 DMA CONT EXTTRIG EXTSEL ADON 설정
	
	*(volatile unsigned int *)0x40012408 |= 0x00400000; // ADC1에서 CR2에서 SWSTART 활성화
	*/
	
	
	
	
	*(volatile unsigned int *)0x40021018 = 0x00000204;  // RCC에서 APB2ENR에서 ADC1EN 활성화
	GPIOA->CRL = 0x00000B00;
	*(volatile unsigned int *)0x40021014 = 0x15;  // RCC에서 AHBENR에서 DM1AEN 활성화
	*(volatile unsigned int *)0x40020008 = 0x000005A0;  // DMA에서 MSIZE, PSIZE 16bit로 설정, DMA에서 CIRC, MINC 설정
	*(volatile unsigned int *)0x4002000C = 0x03; // DNA에서 CNDTR1 카운트 값을 3으로 설정
	*(volatile unsigned int *)0x40020010 = 0x4001244C; // DMA에서 CPAR 값 설정
	*(volatile unsigned int *)0x40020014 = 0x20002000; // DNA에서 CMAR 값 설정
	
	*(volatile unsigned int *)0x40020008 = 0x000005A1;  // DMA에서 enable 설정
	
	*(volatile unsigned int *)0x4001242C = 0x00200000; // ADC1에서 SQR1에서 레벨3으로 설정
	*(volatile unsigned int *)0x40012434 = 0x00001020; // ADC1에서 SQR3에서 시퀀스별 채널 설정(0-1-4)
	*(volatile unsigned int *)0x40012410 = 0x0000502D; // ADC1에서 SMPR2에서 채널 0, 1, 4를 사이클 55.5로 설정
	
	*(volatile unsigned int *)0x40012404 = 0x00000100; // ADC1에서 CR1에서 SCAN 활성화
	*(volatile unsigned int *)0x40012408 = 0x001E0103; // ADC1에서 CR2에서 DMA CONT EXTTRIG EXTSEL ADON 설정
	
	*(volatile unsigned int *)0x40012408 |= 0x00400000; // ADC1에서 CR2에서 SWSTART 활성화
	
	

	for(;;) {
		printf("%p\n", analog);
		//printf("% 4.2fV \r\n", (float)( ( *(unsigned short *)(0x20002000) ) * 3.3 / 0xFFF));
		//printf("% 4.2fV \r\n", (float)(analog[0] * 3.3 / 0xFFF));
		//printf("% 4.2fV , p=%p\r\n", (float)(analog[0] * 3.3 / 0xFFF), &analog[0]);
	}


  }
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
