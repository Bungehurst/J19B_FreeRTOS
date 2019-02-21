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
//�������ȼ�
#define APP_START_TASK_PRIO		1
//�����ջ��С	
#define APP_START_STK_SIZE 		128  
//������
TaskHandle_t APP_Start_Handler;
//������
void APP_Start(void *pvParameters);

//�������ȼ�
#define LED0_TASK_PRIO		2
//�����ջ��С	
#define LED0_STK_SIZE 		50  
//������
TaskHandle_t Led0_Task_Handler;
//������
void Led0_Task(void *pvParameters);

//�������ȼ�
#define SWITCH_TASK_PRIO		3
//�����ջ��С	
#define SWITCH_TASK_STK_SIZE 		50  
//������
TaskHandle_t Switch_Task_Handler;
//������
void Switch_Task(void *pvParameters);


//�������ȼ�
#define LED1_TASK_PRIO		3
//�����ջ��С	
#define LED1_STK_SIZE 		50  
//������
TaskHandle_t Led1_Task_Handler;
//������
void Led1_Task(void *pvParameters);


//�������ȼ�
#define LED2_TASK_PRIO		4
//�����ջ��С	
#define LED2_STK_SIZE 		50  
//������
TaskHandle_t Led2_Task_Handler;
//������
void Led2_Task(void *pvParameters);


//�������ȼ�
#define TIMERCONTROL_TASK_PRIO		2
//�����ջ��С	
#define TIMERCONTROL_STK_SIZE 		256  
//������
TaskHandle_t TimerControlTask_Handler;
//������
void timercontrol_task(void *pvParameters);

TimerHandle_t AutoReloadTimer_Handle;
void AutoReloadCallback(TimerHandle_t xTimer);
int main(void)
{
	BSP_Init();																			//�弶��ʼ��
	PTC_Init();																			//Э����ʼ��
	State = NB_TEST ;
	SEGGER_SYSVIEW_Conf();													//SYSTEMVIEW Monitor
	//������ʼ����
    xTaskCreate((TaskFunction_t )APP_Start,            //������
                (const char*    )"APP_Start",          //��������
                (uint16_t       )APP_START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )APP_START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&APP_Start_Handler);   //������              
    vTaskStartScheduler();          //�����������
		for( ;; );
}
void APP_Start(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    
		AutoReloadTimer_Handle = xTimerCreate((const char*)"timer",
																					(TickType_t)500,
																					(UBaseType_t	)pdTRUE, 	//����
																					(void *)1,
																					(TimerCallbackFunction_t)AutoReloadCallback);
		//����LED0����
    xTaskCreate((TaskFunction_t )Led0_Task,     	
                (const char*    )"Led0_Task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&Led0_Task_Handler);   
		//����LED0����
    xTaskCreate((TaskFunction_t )Led1_Task,     	
                (const char*    )"Led1_Task",   	
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED1_TASK_PRIO,	
                (TaskHandle_t*  )&Led1_Task_Handler);  
		//����LED0����
    xTaskCreate((TaskFunction_t )Led2_Task,     	
                (const char*    )"Led2_Task",   	
                (uint16_t       )LED2_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED2_TASK_PRIO,	
                (TaskHandle_t*  )&Led2_Task_Handler);  
    //����switch����
    xTaskCreate((TaskFunction_t )Switch_Task,     	
                (const char*    )"Switch_Task",   	
                (uint16_t       )SWITCH_TASK_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )SWITCH_TASK_PRIO,	
                (TaskHandle_t*  )&Switch_Task_Handler);  			
		//����TimerControl����
    xTaskCreate((TaskFunction_t )timercontrol_task,             
                (const char*    )"timercontrol_task",           
                (uint16_t       )TIMERCONTROL_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TIMERCONTROL_TASK_PRIO,        
                (TaskHandle_t*  )&TimerControlTask_Handler);  
		vTaskDelete(APP_Start_Handler);
    taskEXIT_CRITICAL();           	 //�˳��ٽ���
		xTimerStart(AutoReloadTimer_Handle,0);
}
void Led0_Task(void *pvParameters)
{
    while(1)
    {
			STM32_LED_SDCARD = !STM32_LED_SDCARD;							  //SDCard LED ��ʱ����ϵͳ����ָʾ��
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
			//STM32_LED_SDCARD = !STM32_LED_SDCARD;							  //SDCard LED ��ʱ����ϵͳ����ָʾ��
			printf("task 2 is running!\r\n");
			vTaskDelay(1000);
    }
}
void Led2_Task(void *pvParameters)
{
    while(1)
    {
			//STM32_LED_SDCARD = !STM32_LED_SDCARD;							  //SDCard LED ��ʱ����ϵͳ����ָʾ��
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



