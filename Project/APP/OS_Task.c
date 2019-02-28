/**
  ******************************************************************************
  * @file    app.c 
  * @author  DPL
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  ******************************************************************************
  */  
/* Includes ------------------------------------------------------------------*/
#include "OS_Task.h"
/* Varibales -----------------------------------------------------------------*/
const u16 REG[]={0xC380,0xD380,0xE380,0xF380};				
u16 RegValue[8] = {0}; 
float Voltage[8] = {0};																
u8 Version[13] = {'Z','1','8','B','_','P','P'					
									,'1','_','V','0','0','0'};
									
char tem = 0;
u8 hum = 0;
float Degree = 0,Vel = 0;
u32 Press = 0;
u16 Pm2 = 0;
u16 Pm10 = 0;
float O3 = 0;
float SO2 = 0;
float CO = 0;
float NO2 = 0;
float NB_Buffer[11]={0};
char T_varible[11][4] = {0};

/* Functions -----------------------------------------------------------------*/

TaskHandle_t APP_Start_Handler;
TaskHandle_t Led0_Task_Handler;
TaskHandle_t TimerControlTask_Handler;
TaskHandle_t Usart3_PM_Decode_Handler;
TimerHandle_t AutoReloadTimer_Handle;
MessageBufferHandle_t BufferFromDma;


const size_t BufferFromDmaSizeBytes = BUFFER_LENTH;

void APP_Start(void *pvParameters)
{
  taskENTER_CRITICAL(); 
	/* Create a MessageBuffer for Usart3 DMA ( PM Sensor ). */
	BufferFromDma = xMessageBufferCreate(BufferFromDmaSizeBytes);
	if(BufferFromDma == NULL)
	{
		printf(" There was not enough heap memory space available to create the message buffer. ");
	}else
	{
		/* The message buffer was created successfully and can now be used. */
	}
	
	
//	AutoReloadTimer_Handle = xTimerCreate((const char*)"timer",
//										  (TickType_t)500,
//										  (UBaseType_t	)pdTRUE, 
//										  (void *)1,
//										  (TimerCallbackFunction_t)AutoReloadCallback);
	
    xTaskCreate((TaskFunction_t )Led0_Task,     	
                (const char*    )"Led0_Task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&Led0_Task_Handler); 	
								
    xTaskCreate((TaskFunction_t )Usart3_PM_Decode_Task,     	
                (const char*    )"Usart3_PM_Decode_Task",   	
                (uint16_t       )USART3_PM_DECODE_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )USART3_PM_DECODE_TASK_PRIO,	
                (TaskHandle_t*  )&Usart3_PM_Decode_Handler);
								
//    xTaskCreate((TaskFunction_t )timercontrol_task,             
//                (const char*    )"timercontrol_task",           
//                (uint16_t       )TIMERCONTROL_STK_SIZE,        
//                (void*          )NULL,                  
//                (UBaseType_t    )TIMERCONTROL_TASK_PRIO,        
//                (TaskHandle_t*  )&TimerControlTask_Handler);  
		vTaskDelete(APP_Start_Handler);
    taskEXIT_CRITICAL();           	 
		//xTimerStart(AutoReloadTimer_Handle,0);
}
void Led0_Task(void *pvParameters)
{
    while(1)
    {
			STM32_LED_SDCARD = !STM32_LED_SDCARD;							 
			printf("task 0 is running!\r\n");
			vTaskDelay(500);
    }
		vTaskDelete(NULL);
}
void timercontrol_task(void *pvParameters)
{
	xTimerStart(AutoReloadTimer_Handle,0);
	vTaskDelete(TimerControlTask_Handler);
}

void AutoReloadCallback( TimerHandle_t xTimer )
{
	
}

void ADS1115_GetData(void)
{	
	taskENTER_CRITICAL();           
	for(int i = 0;i < 4;i++)	
	{
		WindADS1115_WriteToRegister(CONFREG, REG[i]); 	
		if(WINDADS1115_RDYPIN)
		{
			RegValue[i] = WindADS1115_ReadFromRegister(CVSNREG);
		}
	}
	for(int i = 4;i < 8;i++)
	{
		GasADS1115_WriteToRegister(CONFREG, REG[i-4]);
		if(GASADS1115_RDYPIN)
		{
			RegValue[i] = GasADS1115_ReadFromRegister(CVSNREG);
		}
	}
	taskEXIT_CRITICAL();           	 
}

void ADS1115_DataProcess_Task(void *pvParameters)
{
	while(1)
	{
		for(int i = 0; i < 8;i++)
		{
			Voltage[i] = (RegValue[i]*4.096/32768.0);
			if(i==0)
			{
				STM32_LED_WS=!STM32_LED_WS;
				if (Voltage[0]>0&&Voltage[0]<3.0)
					Vel = Voltage[0]*3.67782;										
				else Vel = 0;
			}							
			if(i==1)
			{
				STM32_LED_WD=!STM32_LED_WD;
				if(Voltage[1]>0&&Voltage[1]<3.0)
					Degree = Voltage[1]*165.1376;											
			}						
			if(i==2) 
			{
				STM32_LED_SO2=!STM32_LED_SO2;
				SO2 = Voltage[2];
			}
			if(i==3) 
			{
				STM32_LED_O3=!STM32_LED_O3;
				O3 = Voltage[3];
			}
			if(i==4)
			{
				 //none
			}
			if(i==5)
			{
				STM32_LED_NO2=!STM32_LED_NO2;
				NO2 = Voltage[5];
			}
			if(i==6) 
			{
				STM32_LED_CO=!STM32_LED_CO;
				CO = Voltage[6];
			}
			if(i==7)
			{
				//PH
			}			
		}
	}
	vTaskDelete(NULL);
}	

void Usart3_PM_Decode_Task(void *pvParameters)
{
	u8 buffer_temp[BUFFER_LENTH];
	u8 buffer_inorder[BUFFER_LENTH];
 	u8 buffer_from_message[BUFFER_LENTH];
	size_t xReceiveBytes ;
	const TickType_t xBlockTime = pdMS_TO_TICKS(20);
	while(1)
	{
		xReceiveBytes = xMessageBufferReceive(BufferFromDma,
										  (void * )buffer_from_message,
										  sizeof(buffer_from_message),
										  xBlockTime );
		if( xReceiveBytes > 0 )
		{
			if(buffer_from_message[0]==0xAA && buffer_from_message[1]==0xC0)			
			{
				int u;
				for(u = 0;u < BUFFER_LENTH;u++) buffer_inorder[u]=buffer_from_message[u];
				packet_dec(buffer_inorder);
			}else
			{
				int i,j,k,bit;
				if(buffer_temp[BUFFER_LENTH-1]==0xAA&&buffer_from_message[0]==0xAA)	
				{
					bit = BUFFER_LENTH-1;
				}else 
				{
					for(i = 0;i < BUFFER_LENTH;i++)										
					{
							if(buffer_temp[i]==0xAA && buffer_temp[i+1]==0xAA) bit = i;		
					}
				}
				for(j = 0;j < BUFFER_LENTH-bit;j++)										
				{																											
					buffer_inorder[j]=buffer_temp[j+bit];
				}
				for(k = 0;k < bit;k++)														 	
				{																											
					buffer_inorder[k+BUFFER_LENTH-bit]=buffer_from_message[k];
				}
				for(j = 0;j <BUFFER_LENTH;j++)												
				{
					buffer_temp[j]=buffer_from_message[j];
				}
				packet_dec(buffer_inorder);
			}
			STM32_LED_PM = !STM32_LED_PM;
		}
	}
	vTaskDelete(NULL);
}
/********************************************************************
**Function: packet_dec
**Note 		: 
**Input 	: None
**Output 	: None
**Use 		: packet_dec();
*********************************************************************/
void packet_dec(u8 *buffer)
{
	u16 sum = 0;
	for (int i = 2;i <= 7;i++)
	{
		sum+=buffer[i];
	}
	if((sum&0xff) & buffer[8])
	{
		Pm2  = ((buffer[3]<<8)+buffer[2])/10;//  ug/m3
		Pm10 = ((buffer[5]<<8)+buffer[4])/10;
	}
}
























/********************************************************************
**Function: APP_Start
**Note 		: 
**Input 	: None
**Output 	: None
**Use 		: APP_Start();
*********************************************************************/


//void APP_Start(void)
//{
//	switch(State)
//	{
//		case NB_TEST:
//			for(int i = 0;i < 4;i++)													
//				{	
//					WindADS1115_WriteToRegister(CONFREG, REG[i]); 	
//					if(WINDADS1115_RDYPIN) 													
//					{
//						RegValue[i] = WindADS1115_ReadFromRegister(CVSNREG);
//						Voltage[i] = (RegValue[i]*4.096/32768.0);			

//					}
//					delay_ms(10);																		
//				}

//				//ADC2
//				for(int i = 4;i < 8;i++)													
//				{	
//					GasADS1115_WriteToRegister(CONFREG, REG[i-4]); 		
//					if(GASADS1115_RDYPIN) 														
//					{
//						RegValue[i] = GasADS1115_ReadFromRegister(CVSNREG);
//						Voltage[i] = (RegValue[i]*4.096/32768.0);				
//					}
//					delay_ms(10);																			
//				}
//				
//			SHT31_Read(&tem,&hum);																
//			Press = (u32)BMP180_Update();	
//			
//			NB_Buffer[0] = tem;
//			NB_Buffer[1] = hum;
//			NB_Buffer[2] = Press;
//			NB_Buffer[3] = CO;
//			NB_Buffer[4] = SO2;
//			NB_Buffer[5] = NO2;
//			NB_Buffer[6] = O3;
//			NB_Buffer[7] = Pm2;
//			NB_Buffer[8] = Pm10;
//			NB_Buffer[9] = Vel;
//			NB_Buffer[10] = Degree;
//				
//			
//			USART_Cmd(USART2,DISABLE);										
//			DMA_Cmd(DMA1_Channel6,DISABLE);									
//			memset(&oneNetDevice,0,sizeof(oneNetDevice));
//			memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));	
//			DMA_InitTypeDef dma;
//			DMA_DeInit(DMA1_Channel6);
//			dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
//			dma.DMA_MemoryBaseAddr = (uint32_t)USART2_RX_BUF;
//			dma.DMA_DIR = DMA_DIR_PeripheralSRC;
//			dma.DMA_BufferSize = USART2_REC_LEN;
//			dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//			dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//			dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//			dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//			dma.DMA_Mode = DMA_Mode_Circular;
//			dma.DMA_Priority = DMA_Priority_VeryHigh;
//			dma.DMA_M2M = DMA_M2M_Disable;
//			DMA_Init(DMA1_Channel6,&dma);
//			DMA_Cmd(DMA1_Channel6,ENABLE);
//			USART_Cmd(USART2,ENABLE);	
//			
//			delay_ms(1000);
//			if(err_stat == 0)															
//			{																							
//				RegistrationRequestStatus = NB_SendRegistrationRequest_2();
//			}else if(err_stat == 1)												
//			{																							
//				USART_Cmd(USART2,DISABLE);									
//				DMA_Cmd(DMA1_Channel6,DISABLE);						
//				NB_IOT_RESET = 0;														
//				delay_ms(500);
//				NB_IOT_RESET = 1;
//				for(int i = 0; i< 10 ;i++) delay_ms(1000);	//10s
//				USART_Cmd(USART2,ENABLE);										
//				DMA_Cmd(DMA1_Channel6,ENABLE);							
//				NB_InitConnectOneNet();	
//				NB_DeleteRegistrationRequest();
//				delay_ms(1000);delay_ms(1000);
//				NB_SendRegistrationRequest();
//				delay_ms(1000);
//				printf("AT+MIPLCLOSE=0\r\n");
//				printf("AT+MIPLCLOSE=0\r\n");
//				delay_ms(1000);
//				memset(&oneNetDevice,0,sizeof(oneNetDevice));
//				memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));	
//				delay_ms(1000);
//				RegistrationRequestStatus = 0;
//				err_stat = 0;
//				break;
//			}
//			//delay_ms(1000);
//			if(RegistrationRequestStatus == 2) 
//			{
//				sprintf(T_varible[0],"%f",NB_Buffer[0]);
//				NB_NotifyOneNetMsg((const char *)oneNetDevice.observeMsgId_3303_0,
//													 (uint8_t *)"3303",
//													 (uint8_t *)"0",
//													 (uint8_t *)"5700",
//													 (uint8_t *)T_varible[0]);

//				sprintf(T_varible[1],"%f",NB_Buffer[1]);
//				NB_NotifyOneNetMsg((const char *)oneNetDevice.observeMsgId_3304_0,(uint8_t *)"3304",(uint8_t *)"0",(uint8_t *)"5700",(uint8_t *)T_varible[1]);

//				sprintf(T_varible[2],"%4.1f",NB_Buffer[2]);
//				NB_NotifyOneNetMsg((const char *)oneNetDevice.observeMsgId_3323_0,(uint8_t *)"3323",(uint8_t *)"0",(uint8_t *)"5700",(uint8_t *)T_varible[2]);

//				sprintf(T_varible[3],"%f",NB_Buffer[3]);
//				NB_NotifyOneNetMsg((const char *)oneNetDevice.observeMsgId_3315_0,(uint8_t *)"3315",(uint8_t *)"0",(uint8_t *)"5700",(uint8_t *)T_varible[3]);

//				sprintf(T_varible[4],"%f",NB_Buffer[4]);
//				NB_NotifyOneNetMsg((const char *)oneNetDevice.observeMsgId_3315_1,(uint8_t *)"3315",(uint8_t *)"1",(uint8_t *)"5700",(uint8_t *)T_varible[4]);

//				sprintf(T_varible[5],"%f",NB_Buffer[5]);
//				NB_NotifyOneNetMsg((const char *)oneNetDevice.observeMsgId_3315_2,(uint8_t *)"3315",(uint8_t *)"2",(uint8_t *)"5700",(uint8_t *)T_varible[5]);

//				sprintf(T_varible[6],"%f",NB_Buffer[6]);
//				NB_NotifyOneNetMsg((const char *)oneNetDevice.observeMsgId_3315_3,(uint8_t *)"3315",(uint8_t *)"3",(uint8_t *)"5700",(uint8_t *)T_varible[6]);

//				sprintf(T_varible[7],"%f",NB_Buffer[7]);
//				NB_NotifyOneNetMsg((const char *)oneNetDevice.observeMsgId_3316_0,(uint8_t *)"3316",(uint8_t *)"0",(uint8_t *)"5700",(uint8_t *)T_varible[7]);

//				sprintf(T_varible[8],"%f",NB_Buffer[8]);
//				NB_NotifyOneNetMsg((const char *)oneNetDevice.observeMsgId_3316_1,(uint8_t *)"3316",(uint8_t *)"1",(uint8_t *)"5700",(uint8_t *)T_varible[8]);

//				sprintf(T_varible[9],"%f",NB_Buffer[9]);
//				NB_NotifyOneNetMsg((const char *)oneNetDevice.observeMsgId_3316_2,(uint8_t *)"3316",(uint8_t *)"2",(uint8_t *)"5700",(uint8_t *)T_varible[9]);

//				sprintf(T_varible[10],"%f",NB_Buffer[10]);
//				NB_NotifyOneNetMsg((const char *)oneNetDevice.observeMsgId_3316_3,(uint8_t *)"3316",(uint8_t *)"3",(uint8_t *)"5700",(uint8_t *)T_varible[10]);
//				
//				delay_ms(1000);
//				memset(&oneNetDevice,0,sizeof(oneNetDevice));
//				NB_SendCmd((uint8_t *)"AT+MIPLCLOSE=0\r\n",(uint8_t *)"+MIPLEVENT: 0",DefaultTimeout,1);
//				RegistrationRequestStatus = 0;
//				USART_Cmd(USART2,DISABLE);	
//				DMA_Cmd(DMA1_Channel6,DISABLE);	
//				break;
//			}
//			if(RegistrationRequestStatus == 0) err_stat = 1;
//			break;
//		default: break;
//		}
//}
