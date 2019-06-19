#include"osKernel.h"
#include"stm32f4_rtos_bsp.h"

#define QUANTA 		1

int32_t semaphore1,semaphore2;

uint32_t count0,count1,count2 ; 


void Task0(void)
{
	while(1)
	{
		count0++;
//		osThreadYield();
	}
}

void Task1(void)
{
	while(1)
	{
		osSignalWait(&semaphore2);
		ST7735_DrawString(3,7,"This is THREAD 1 ",GREEN);
		osSignalSet(&semaphore1);

	}
	
}

void Task2(void)
{
	while(1)
	{
		osSignalWait(&semaphore1);
		ST7735_DrawString(3,5,"This is THREAD 2 ",GREEN);
		osSignalSet(&semaphore2);
	}
	
}

void Task3(void)
{
Probe_CH3();
}


/*
RENDEVOUS
void Task1(void)
{
	while(1)
	{
		osSignalSet(&semaphore1);
		osSignalWait(&semaphore2);
		ST7735_DrawString(3,7,"This is THREAD 1 ",GREEN);

	}
	
}

void Task2(void)
{
	while(1)
	{
		osSignalSet(&semaphore2);
		osSignalWait(&semaphore1);
		ST7735_DrawString(3,5,"This is THREAD 2 ",GREEN);
		
	}
	
}
*/

int main(void)
{
	osSemaphoreInit(&semaphore1,1);
	osSemaphoreInit(&semaphore2,0);

	HAL_Init();
	Probe_Init();
	TIM4_Init_Start();
	ST7735_Init();
	osKernelInit();
	osKernelAddThreads(&Task0,&Task1,&Task2);
	osKernelLaunch(QUANTA);
	
	return 0 ; 
}


void TIM4_IRQHandler(void)
{
	Probe_CH3();
	HAL_TIM_IRQHandler(&htim4);
	
}
