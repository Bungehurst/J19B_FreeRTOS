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
#include "app.h"
/* Varibales -----------------------------------------------------------------*/
const u16 REG[]={0xC380,0xD380,0xE380,0xF380};				//ADS1115四个通道寄存器地址
u16 RegValue[8] = {0}; 
float Voltage[8] = {0};																//四个通道寄存器值
u8 Version[13] = {'Z','1','8','B','_','P','P'					//版本信息
									,'1','_','V','0','0','0'};
									
//传感器变量
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

/********************************************************************
**Function: APP_Start
**Note 		: 应用层入口
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
//					WindADS1115_WriteToRegister(CONFREG, REG[i]); 	//开始读写
//					if(WINDADS1115_RDYPIN) 													//如果转换完成 Pin为高电平
//					{
//						RegValue[i] = WindADS1115_ReadFromRegister(CVSNREG);//读取寄存器值
//						Voltage[i] = (RegValue[i]*4.096/32768.0);			//转换成实际电压
//						if(i==0)
//						{
//							STM32_LED_WS=!STM32_LED_WS;
//							if (Voltage[0]>0&&Voltage[0]<3.0)
//								Vel = Voltage[0]*3.67782;										//转换为速度  方法请参见产品文档
//							else Vel = 0;
//						}							
//						if(i==1)
//						{
//							STM32_LED_WD=!STM32_LED_WD;
//							if(Voltage[1]>0&&Voltage[1]<3.0)
//								Degree = Voltage[1]*165.1376;											//转换为角度  方法请参见产品文档
//						}						
//						if(i==2) 
//						{
//							STM32_LED_SO2=!STM32_LED_SO2;
//							SO2 = Voltage[2];
//						}
//						if(i==3) 
//						{
//							STM32_LED_O3=!STM32_LED_O3;
//							O3 = Voltage[3];
//						}
//					}
//					delay_ms(10);																		//通道切换时间10ms (必需)
//				}

//				//ADC2
//				for(int i = 4;i < 8;i++)													
//				{	
//					GasADS1115_WriteToRegister(CONFREG, REG[i-4]); 		//开始读写
//					if(GASADS1115_RDYPIN) 														//如果转换完成 Pin为高电平
//					{
//						RegValue[i] = GasADS1115_ReadFromRegister(CVSNREG); //读取寄存器值
//						Voltage[i] = (RegValue[i]*4.096/32768.0);				//转换成实际电压
//						if(i==4)
//						{
//							//空
//						}
//						if(i==5)
//						{
//							STM32_LED_NO2=!STM32_LED_NO2;
//							NO2 = Voltage[5];
//						}
//						if(i==6) 
//						{
//							STM32_LED_CO=!STM32_LED_CO;
//							CO = Voltage[6];
//						}
//						if(i==7)
//						{
//							//PH
//						}
//					}
//					delay_ms(10);																			//通道切换时间10ms (必需)
//				}
//				
//			SHT31_Read(&tem,&hum);																//读取SHT31
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
//			//重新初始化DMA，使数据从BUFFER头更新
//			USART_Cmd(USART2,DISABLE);											//失能串口
//			DMA_Cmd(DMA1_Channel6,DISABLE);									//失能DMA
//			memset(&oneNetDevice,0,sizeof(oneNetDevice));
//			memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));	//清缓存
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
//			if(err_stat == 0)															//如果模块没有问题 err_stat == 0
//			{																							//不删除注册信息，只打开链路
//				RegistrationRequestStatus = NB_SendRegistrationRequest_2();
//			}else if(err_stat == 1)												//如果模块有问题 err_stat == 1
//			{																							//删除注册信息，重新配置
//				USART_Cmd(USART2,DISABLE);									//失能串口
//				DMA_Cmd(DMA1_Channel6,DISABLE);							//失能DMA
//				NB_IOT_RESET = 0;														//NB模块硬件复位
//				delay_ms(500);
//				NB_IOT_RESET = 1;
//				for(int i = 0; i< 10 ;i++) delay_ms(1000);	//10s
//				USART_Cmd(USART2,ENABLE);										//失能串口
//				DMA_Cmd(DMA1_Channel6,ENABLE);							//失能DMA
//				//重新注册对象
//				NB_InitConnectOneNet();	
//				NB_DeleteRegistrationRequest();
//				delay_ms(1000);delay_ms(1000);
//				NB_SendRegistrationRequest();
//				delay_ms(1000);
//				printf("AT+MIPLCLOSE=0\r\n");
//				printf("AT+MIPLCLOSE=0\r\n");
//				delay_ms(1000);
//				memset(&oneNetDevice,0,sizeof(oneNetDevice));
//				memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));	//清缓存
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
