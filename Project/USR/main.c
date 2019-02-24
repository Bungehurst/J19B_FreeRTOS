/**
  ******************************************************************************
  * @file    main.c 
  * @author  DPL
  * @version 
  * @date    
  * @brief 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Main function -------------------------------------------------------------*/
int main(void)
{
	/* Board Support Package Initialize. */
	BSP_Init();
	/* Protocal Initialize. */
	PTC_Init();							
	/* Set system status to NB mode. */
	State = NB_TEST ;
	/* SystemView Monitor Initialize. */
	SEGGER_SYSVIEW_Conf();
	/* Create a start task. */
  xTaskCreate((TaskFunction_t )APP_Start,            
              (const char*    )"APP_Start",         
              (uint16_t       )APP_START_STK_SIZE,       
              (void*          )NULL,                  
              (UBaseType_t    )APP_START_TASK_PRIO,       
              (TaskHandle_t*  )&APP_Start_Handler);        
	/* Start Task Scheduler. */
  vTaskStartScheduler();  
	/* Execution will only reach here if there was insufficient heap to start the scheduler. */
	for( ;; );
	return 0;
}




