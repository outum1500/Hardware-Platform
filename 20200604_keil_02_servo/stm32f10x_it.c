/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED             ( 1 << 6 )                // PB5: LED D2
#define LED1             ( 1 << 13 )                // PB5: LED D2
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
void TimingDelay_Decrement(void);
void pwm_toggle();
void SysTick_Handler(void);
unsigned long micros_10us();
volatile unsigned long count = 0;
int timer_count = 0;
int ledFlag = 0;
int pwm_duty;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */

void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
	count++;
}

void TIM2_IRQHandler (void) {   // 100us 마다 접속

  if ((TIM2->SR & 0x0001) != 0) {                 // check interrupt source

	timer_count++;
	if(timer_count >= 100){
		timer_count=0;
		GPIOA->ODR |= (0x01 << 5); 
	}
	else if(timer_count == pwm_duty){
		GPIOA->ODR &= ~(0x01 << 5);
	}

    TIM2->SR &= ~(1<<0);                          // clear UIF flag
 }
} // end TIM2_UP_IRQHandler

void EXTI0_IRQHandler(void)
{
	
  if (EXTI->PR & (1<<0)) {                        // EXTI0 interrupt pending?


    EXTI->PR = (1<<0);                           // clear pending interrupt
  }
}

void EXTI15_10_IRQHandler(void)
{
  if (EXTI->PR & (1<<13)) {                       // EXTI0 interrupt pending?

    EXTI->PR = (1<<13);                          // clear pending interrupt
  }
}

void EXTI9_5_IRQHandler(void)
{

	
  if (EXTI->PR & (1<<5)) {                       // EXTI0 interrupt pending?

    EXTI->PR = (1<<5);                          // clear pending interrupt
  }
	
	 else if (EXTI->PR & (1<<6)) {                       // EXTI0 interrupt pending?

    EXTI->PR = (1<<6);                          // clear pending interrupt
  }
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
