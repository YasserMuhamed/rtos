

#include"stm32f4xx.h"


#define RED 										(1U<<14)
#define GREEN 									(1U<<12)
#define BLUE 										(1U<<15)
#define ORANGE 									(1U<<13)


#define RED_BIT									  	(1U<<28)
#define GREEN_BIT 									(1U<<24)
#define BLUE_BIT 										(1U<<26)
#define ORANGE_BIT 									(1U<<30)


volatile uint32_t tick;
volatile uint32_t _tick;
uint32_t orange_stack[40];
uint32_t blue_stack[40]; 
uint32_t *sp_orange = &orange_stack[40];
uint32_t *sp_blue=&blue_stack[40];

	
#define GPIOD_CLOCK									(1<<3)


void GPIO_Init(void);
void DelayS(uint32_t seconds );
void blueOn(void);
void blueOff(void);
void orangeOn(void);
void orangeOff(void);

int orange_main(void)
{
	while(1)
	{
		orangeOn();
		DelayS(1);
		orangeOff();
		DelayS(1);
	}
}

int blue_main(void)
{
	while(1)
	{
		blueOn();
		DelayS(1);
		blueOff();
		DelayS(1);
	}
}
int main(void)
{
	uint32_t volatile start=0U;
	*(--sp_orange)=(1U<<24);/*XPSR*/
	*(--sp_orange)=(uint32_t)&orange_main;/*PC*/
	*(--sp_orange)=0x0000000DU;;/*Link Register*/
	*(--sp_orange)=0x0000000EU;/*R12*/
	*(--sp_orange)=0x0000000AU;;/*R3*/
	*(--sp_orange)=0x0000000DU;/*R2*/
   // STACK FOR BLUE MAIN THREAD
	*(--sp_blue)=(1U<<24);/*XPSR*/
	*(--sp_blue)=(uint32_t)&blue_main;/*PC*/
	*(--sp_blue)=0x0000000DU;;/*Link Register*/
	*(--sp_blue)=0x0000000EU;/*R12*/
	*(--sp_blue)=0x0000000AU;;/*R3*/
	*(--sp_blue)=0x0000000DU;/*R2*/
	

	
	
	GPIO_Init();
	 while(1)
	{
	if(start)
	blue_main();
	else
	orange_main();


	}
}


void GPIO_Init(void)
{
	RCC->AHB1ENR|=GPIOD_CLOCK;
	GPIOD->MODER^=RED_BIT|GREEN_BIT|ORANGE_BIT|BLUE_BIT;
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/100U);
	__enable_irq();
}

void SysTick_Handler(void)
{
	
	++tick;
	
}

uint32_t getTick(void)
{
	__disable_irq();
	_tick=tick;
	__enable_irq();
	return _tick;
}

void DelayS(uint32_t seconds )
{
	seconds *=100;
	uint32_t temp=getTick();
	while((getTick()-temp)<seconds);
}

void blueOn(void)
{
	GPIOD->ODR|=BLUE;
}

void blueOff(void)
{
	GPIOD->ODR&=~BLUE;
}

void orangeOn(void)
{
	GPIOD->ODR|=ORANGE;
}

void orangeOff(void)
{
	GPIOD->ODR&=~ORANGE;
}
