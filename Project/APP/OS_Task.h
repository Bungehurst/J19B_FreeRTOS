#ifndef __APP_H
#define __APP_H

#include "includes.h"

#include "usart3_pm.h"

#define APP_START_TASK_PRIO					1
#define LED0_TASK_PRIO							2
#define TIMERCONTROL_TASK_PRIO			2
#define SWITCH_TASK_PRIO						3
#define LED1_TASK_PRIO							3
#define LED2_TASK_PRIO							4



#define APP_START_STK_SIZE 					128  
#define LED0_STK_SIZE 							50  
#define SWITCH_TASK_STK_SIZE 				50 
#define LED1_STK_SIZE 							50  
#define LED2_STK_SIZE 							50 
#define TIMERCONTROL_STK_SIZE 			256 



extern TaskHandle_t APP_Start_Handler;
extern TaskHandle_t Led0_Task_Handler;
extern TaskHandle_t Switch_Task_Handler;
extern TaskHandle_t Led1_Task_Handler;
extern TaskHandle_t Led2_Task_Handler;
extern TaskHandle_t TimerControlTask_Handler;
extern TimerHandle_t AutoReloadTimer_Handle;


void APP_Start(void *pvParameters);
void Led0_Task(void *pvParameters);
void Switch_Task(void *pvParameters);
void Led1_Task(void *pvParameters);
void Led2_Task(void *pvParameters);
void timercontrol_task(void *pvParameters);
void AutoReloadCallback(TimerHandle_t xTimer);

#endif
