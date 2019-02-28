#ifndef __OS_TASK_H
#define __OS_TASK_H

/* Headers. */
#include "includes.h"

/* Prioity of tasks. */
#define APP_START_TASK_PRIO					1
#define LED0_TASK_PRIO							2
#define TIMERCONTROL_TASK_PRIO			2
#define USART3_PM_DECODE_TASK_PRIO	3

/* Stack occupancy of tasks. */
#define APP_START_STK_SIZE 					128  
#define LED0_STK_SIZE 							50  
#define TIMERCONTROL_STK_SIZE 			256 
#define USART3_PM_DECODE_STK_SIZE 	256 

/* Task Handlers. */
extern TaskHandle_t APP_Start_Handler;
extern TaskHandle_t Led0_Task_Handler;
extern TaskHandle_t TimerControlTask_Handler;
extern TaskHandle_t Usart3_PM_Decode_Handler;

extern TimerHandle_t AutoReloadTimer_Handle;

extern MessageBufferHandle_t BufferFromDma;


/* Task Functions. */
void APP_Start(void *pvParameters);
void Led0_Task(void *pvParameters);
void timercontrol_task(void *pvParameters);
void AutoReloadCallback(TimerHandle_t xTimer);
void Usart3_PM_Decode_Task(void *pvParameters);
void packet_dec(u8 *buffer);



#endif
