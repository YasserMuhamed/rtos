

#include "osKernel.h"

#define  NUM_OF_THREADS		  	3
#define  STACKSIZE						100
#define  BUS_FREQ						  16000000
#define  SYSPRI3							(*((volatile uint32_t*)0XE000ED20))
#define  INTCTRL							(*((volatile uint32_t *) 0xE000ED04))
uint32_t MILLIS_PRESCALER;

void osSchedulerLaunch(void);
typedef struct tcb
{
	int32_t *stackPt;
	struct tcb *nextPt;
}tcbType;

tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;

int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void osKernelStackInit(int i)
{
	tcbs[i].stackPt=&TCB_STACK[i][STACKSIZE-16]; 	  	// initialize the stack pointer
	TCB_STACK[i][STACKSIZE-1]=0x01000000; 					 //initialize xpsr register for thumb mode
}

uint8_t  osKernelAddThreads(void (*task0)(void),void (*task1)(void),void (*task2)(void))
{
	__disable_irq();//disable interrupt, critical section coming
	
	tcbs[0].nextPt=&tcbs[1];
	tcbs[1].nextPt=&tcbs[2];
	tcbs[2].nextPt=&tcbs[0];
	
	
	osKernelStackInit(0);
	//PROGRAM COUNTER LOCATION
	TCB_STACK[0][STACKSIZE-2]=(int32_t)(task0);

		osKernelStackInit(1);
	//PROGRAM COUNTER LOCATION
	TCB_STACK[1][STACKSIZE-2]=(int32_t)(task1);
	
		osKernelStackInit(2);
	//PROGRAM COUNTER LOCATION
	TCB_STACK[2][STACKSIZE-2]=(int32_t)(task2);
	currentPt=&tcbs[0];
	__enable_irq();
	
	return 1 ;
}


void osKernelInit(void)
{
	__disable_irq();
	MILLIS_PRESCALER=BUS_FREQ/1000;
}


void osKernelLaunch(uint32_t quanta)
{
	SysTick->CTRL=0; //disable systick
	SysTick->VAL=0;  //initialize systick
	SYSPRI3=(SYSPRI3&0X00FFFFFF)|0XE00000C0; //Priority 7
	SysTick->LOAD=(quanta*MILLIS_PRESCALER)-1;
	SysTick->CTRL=0X00000007; //enable systick
	osSchedulerLaunch();
}

void osThreadYield(void)
{
	SysTick->VAL=0;  //reset the value of sysTick
	INTCTRL=0x04000000; //trigger SysTick
}	


//LCM of 100 and 250 = 500
void osSchedulerRoundRobin(void)
{
	if(++periodic_tick==PERIOD)
	{
		(*Task3)();
		periodic_tick=0;
	}
	currentPt=currentPt->nextPt;
}

void osSemaphoreInit(int32_t *semaphore, int32_t value)
{
	*semaphore = value;
}

void osSignalSet(int32_t *semaphore)
{
	__disable_irq();
	*semaphore+=1;
	__enable_irq();
}

void osSignalWait(int32_t *semaphore)
{
	__disable_irq();
	while(*semaphore<=0)
	{
		__disable_irq();
		osThreadYield();
		__enable_irq();
	}
	*semaphore-=1;
	__enable_irq();
}

//Scheduler
//TCB 
//Stack 


