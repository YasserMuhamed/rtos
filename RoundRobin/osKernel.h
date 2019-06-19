#ifndef _OS_KERNEL_H 
#define _OS_KERNEL_H 


#include"stdint.h"
#include "stm32f4xx.h"                  // Device header

void osSemaphoreInit(int32_t *semaphore, int32_t value);
void osSignalSet(int32_t *semaphore);
void osSignalWait(int32_t *semaphore);
void osThreadYield(void);
void osKernelLaunch(uint32_t quanta);
void osKernelInit(void);
uint8_t  osKernelAddThreads(void (*task0)(void),
														void (*task1)(void),
														void (*task2)(void));


void osKernelStackInit(int i);

void osSchedulerRoundRobin(void);


uint32_t periodic_tick;
#define PERIOD		100

void Task3(void);



#endif