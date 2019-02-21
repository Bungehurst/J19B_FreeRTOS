/**
  ******************************************************************************
  * @file    ptc.c 
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  ******************************************************************************
  */  
/* Includes ------------------------------------------------------------------*/
#include "ptc.h"
/* Varibales ------------------------------------------------------------------*/
u8 Tansfer_Buffer[30];
u32 count = 0;
uint8_t RegistrationRequestStatus=0;
/* Functions -----------------------------------------------------------------*/


/********************************************************************
**Function: PTC_Init
**Note 		: 协议层初始化
**Input 	: None
**Output 	: None
**Use 		: PTC_Init();
*********************************************************************/

void PTC_Init(void)
{
	//IIC_ADS初始化
	IIC_ADS_Init();				
	if(GasADS1115_Init(CONFREG_INIT, LOREG_INIT, HIREG_INIT) == 0 ) 	STM32_LED_ADC1 = 0;				//ADS1115_Gas  初始化
	else Err_State = IIC_FAULT;
	delay_ms(1);
	if(WindADS1115_Init(CONFREG_INIT, LOREG_INIT, HIREG_INIT) == 0)	 	STM32_LED_ADC2 = 0;			  //ADS1115_Wind 初始化
	else Err_State = IIC_FAULT;
	delay_ms(1);
	
	//IIC_HTS初始化
	IIC_HTS_Init();			 	
//	
//	for(int i = 0;i < 10;i++) delay_ms(1000);		//延迟20s等待模块开机
//	
//	DMA_Cmd(DMA1_Channel6,ENABLE);	
//	USART_Cmd(USART2,ENABLE);	
//	
//	//BC-35G NB-IoT 初始化
//	NB_InitConnectOneNet();											//检查模块状态
//	if(RegistrationRequestStatus == 0)
//	{
//		NB_DeleteRegistrationRequest();						//清空注册信息
//		delay_ms(1000);delay_ms(1000);						//2s
//		RegistrationRequestStatus = NB_SendRegistrationRequest();		//注册
//	}

//	if(RegistrationRequestStatus == 2)
//	{
//		printf("AT+MIPLCLOSE=0\r\n");							//关闭链路
//		printf("AT+MIPLCLOSE=0\r\n");
//	}
//	
//	memset(&oneNetDevice,0,sizeof(oneNetDevice));		//清结构体
//	memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));	//清缓存
//	
//	
//	for(int i = 0;i < 5;i++) delay_ms(1000);		//延迟10s
//	
//	count = 0;																			//计数清零
}


