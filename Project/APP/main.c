/**
  ******************************************************************************
  * @file    main.c 
  * @author  DPL
  * @version V1.0.0
  * @date    
  * @brief   
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
  * 										COPYRIGHT 2011 STMicroelectronics
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "SEGGER_SYSVIEW.h"
#include "timers.h"
/* Main function -------------------------------------------------------------*/
//任务优先级
#define APP_START_TASK_PRIO		1
//任务堆栈大小	
#define APP_START_STK_SIZE 		128  
//任务句柄
TaskHandle_t APP_Start_Handler;
//任务函数
void APP_Start(void *pvParameters);

//任务优先级
#define LED0_TASK_PRIO		2
//任务堆栈大小	
#define LED0_STK_SIZE 		50  
//任务句柄
TaskHandle_t Led0_Task_Handler;
//任务函数
void Led0_Task(void *pvParameters);

//任务优先级
#define SWITCH_TASK_PRIO		3
//任务堆栈大小	
#define SWITCH_TASK_STK_SIZE 		50  
//任务句柄
TaskHandle_t Switch_Task_Handler;
//任务函数
void Switch_Task(void *pvParameters);


//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		50  
//任务句柄
TaskHandle_t Led1_Task_Handler;
//任务函数
void Led1_Task(void *pvParameters);


//任务优先级
#define LED2_TASK_PRIO		4
//任务堆栈大小	
#define LED2_STK_SIZE 		50  
//任务句柄
TaskHandle_t Led2_Task_Handler;
//任务函数
void Led2_Task(void *pvParameters);


//任务优先级
#define TIMERCONTROL_TASK_PRIO		2
//任务堆栈大小	
#define TIMERCONTROL_STK_SIZE 		256  
//任务句柄
TaskHandle_t TimerControlTask_Handler;
//任务函数
void timercontrol_task(void *pvParameters);

TimerHandle_t AutoReloadTimer_Handle;
void AutoReloadCallback(TimerHandle_t xTimer);
int main(void)
{
	BSP_Init();																			//板级初始化
	PTC_Init();																			//协议层初始化
	State = NB_TEST ;
	SEGGER_SYSVIEW_Conf();													//SYSTEMVIEW Monitor
	//创建开始任务
    xTaskCreate((TaskFunction_t )APP_Start,            //任务函数
                (const char*    )"APP_Start",          //任务名称
                (uint16_t       )APP_START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )APP_START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&APP_Start_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
		for( ;; );
}
void APP_Start(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    
		AutoReloadTimer_Handle = xTimerCreate((const char*)"timer",
																					(TickType_t)500,
																					(UBaseType_t	)pdTRUE, 	//周期
																					(void *)1,
																					(TimerCallbackFunction_t)AutoReloadCallback);
		//创建LED0任务
    xTaskCreate((TaskFunction_t )Led0_Task,     	
                (const char*    )"Led0_Task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&Led0_Task_Handler);   
		//创建LED0任务
    xTaskCreate((TaskFunction_t )Led1_Task,     	
                (const char*    )"Led1_Task",   	
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED1_TASK_PRIO,	
                (TaskHandle_t*  )&Led1_Task_Handler);  
		//创建LED0任务
    xTaskCreate((TaskFunction_t )Led2_Task,     	
                (const char*    )"Led2_Task",   	
                (uint16_t       )LED2_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED2_TASK_PRIO,	
                (TaskHandle_t*  )&Led2_Task_Handler);  
    //创建switch任务
    xTaskCreate((TaskFunction_t )Switch_Task,     	
                (const char*    )"Switch_Task",   	
                (uint16_t       )SWITCH_TASK_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )SWITCH_TASK_PRIO,	
                (TaskHandle_t*  )&Switch_Task_Handler);  			
		//创建TimerControl任务
    xTaskCreate((TaskFunction_t )timercontrol_task,             
                (const char*    )"timercontrol_task",           
                (uint16_t       )TIMERCONTROL_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TIMERCONTROL_TASK_PRIO,        
                (TaskHandle_t*  )&TimerControlTask_Handler);  
		vTaskDelete(APP_Start_Handler);
    taskEXIT_CRITICAL();           	 //退出临界区
		xTimerStart(AutoReloadTimer_Handle,0);
}
void Led0_Task(void *pvParameters)
{
    while(1)
    {
			STM32_LED_SDCARD = !STM32_LED_SDCARD;							  //SDCard LED 暂时用作系统运行指示灯
			printf("task 0 is running!\r\n");
			vTaskDelay(500);
    }
}
void Switch_Task(void *pvParameters)
{
	while(1)
	{
		printf("task 1 is running!\r\n");
		vTaskDelay(1500);
	}

}
void Led1_Task(void *pvParameters)
{
    while(1)
    {
			//STM32_LED_SDCARD = !STM32_LED_SDCARD;							  //SDCard LED 暂时用作系统运行指示灯
			printf("task 2 is running!\r\n");
			vTaskDelay(1000);
    }
}
void Led2_Task(void *pvParameters)
{
    while(1)
    {
			//STM32_LED_SDCARD = !STM32_LED_SDCARD;							  //SDCard LED 暂时用作系统运行指示灯
			printf("task 3 is running!\r\n");
			vTaskDelay(30);
    }
}
void timercontrol_task(void *pvParameters)
{
		xTimerStart(AutoReloadTimer_Handle,0);
		vTaskDelete(TimerControlTask_Handler);
}

void AutoReloadCallback( TimerHandle_t xTimer )
{
	
}



