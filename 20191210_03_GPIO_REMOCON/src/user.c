

// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][] //
// Delay_10us(50) 			=> delay 50us
// Delay_ms(30) 			=> delay 30ms
// CheckMillis() 			=> check elapsed time(us) after POR
// CheckMicros() 			=> check elapsed time(ms) after POR
// printf(format)   		=> print using UART
// gpioA_High[Low](pin#)	=> output high[Low] on PA, pin#
// gpioB_High[Low](pin#)	=> output high[Low] on PB, pin#

// attachInterrupt(pin#, userFuncName, intMode)
//							=>	GPIOC pin# 	 : 10, 11, 12, 13, 14, 15
//								userFuncName : user function name
//                                             (which is called in EXTI IRQ Handler) 
// 								intMode		 :	EXTI_Trigger_Rising
//				  							    EXTI_Trigger_Falling 
//				 							    EXTI_Trigger_Rising_Falling

// Init_Timer2UpdateInterrupt(20) => Timer2 update interrupt per 20us
// UserFnTime2()				  => user function (called in TIM2 IRQ Handler)

// Init_Timer3UpdateInterrupt(50) => Timer3 update interrupt per 50us
// UserFnTime3()				  => user function (called in TIM3 IRQ Handler)


// pinMode(port, pin, in/out) => set pin as input or output 
//								port : GPIO_A, GPIO_B, GPIO_C
//								pin : each 0 ~ 15 or ALL_PIN
//								in/out : INPUT or OUTPUT
// digitalWriteBit(port, pin, bitValue) 
//							=> port : GPIO_A, GPIO_B, GPIO_C
//							   pin  : 0 ~ 15
//							   bitValue : HIGH, LOW (or 1 / 0)
// digitalWritePort(port, portValue)
//							=> port : GPIO_A, GPIO_B, GPIO_C
//                             portValue : port value  
// digitalReadBit(port, pin) => bit read
// digitalReadPort(port)     => port read
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][] //




#include "user.h"

void aaa(void);
int tmp_a=3;

volatile void userFn15(void);
volatile void userFn14(void);
volatile void userFn13(void);
volatile void userFn12(void);
volatile void userFn11(void);
volatile void userFn10(void);

//inline void userFnTim2(void);
volatile void userFnTim2(void);
volatile void userFnTim3(void);

// for Timer interrupt test (toggle based on these value)
volatile unsigned char state0 = 0, state1 = 0;

// for digital read / write test
unsigned long currentMillis = 0, previousMillis0 = 0, previousMillis1 = 0;
unsigned short portVal;
int direction = 1;


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void setup()
{
	previousMillis0 = previousMillis1 = CheckMillis();
}


 
void loop()
{
	currentMillis = CheckMillis();


}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++





///////////////////////////////////////////////
void aaa()
{
	tmp_a = 5;
	printf("tmp_a = %d..\n",tmp_a);

}

///////////////////////////////////////////////
// user function to be serviced when external interrupt-GPIOC.15 occur
// called in EXTI IRQ Handler
volatile void userFn15(void)
{
	printf("External Interrupt GPIOC.15.\n");
}

volatile void userFn14(void)
{
	printf("External Interrupt GPIOC.14.\n");
}

volatile void userFn13(void)
{
	printf("External Interrupt GPIOC.13.\n");
}

volatile void userFn12(void)
{
	printf("External Interrupt GPIOC.12.\n");
}

volatile void userFn11(void)
{
	printf("External Interrupt GPIOC.11.\n");
}

volatile void userFn10(void)
{
	if(  !digitalReadBit(GPIO_C, 10) ){
		
		digitalWriteBit(GPIO_A, 0, HIGH);
	}
}
///////////////////////////////////////////////






///////////////////////////////////////////////
// user function to be serviced when timer2  interrupt occur
// called in TIM2_IRQ Handler
volatile void userFnTim2(void)
{
	state0 = !state0;
	if(state0)
		digitalWriteBit(GPIO_B, 0, HIGH);
	else
		digitalWriteBit(GPIO_B, 0, LOW);
}

// user function to be serviced when timer3  interrupt occur
// called in TIM3_IRQ Handler
volatile void userFnTim3(void)
{
	state1 = !state1;
	if(state1)
		digitalWriteBit(GPIO_B, 1, HIGH);
	else
		digitalWriteBit(GPIO_B, 1, LOW);
}
///////////////////////////////////////////////









