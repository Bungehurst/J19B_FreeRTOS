/**
  ******************************************************************************
  * @file    usart3_pm.c 
  * @author  DPL
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  ******************************************************************************
  */  
/* Includes ------------------------------------------------------------------*/
#include "usart3_pm.h"

#include "includes.h"

/* Varibales -----------------------------------------------------------------*/
u8 buffer_usart3[BUFFER_LENTH];
u8 buffer_temp[BUFFER_LENTH];
u8 buffer_inorder[BUFFER_LENTH];
/********************************************************************
**Function: USART3_PM_Init
**Note 		: USART3初始化及配置，USART3是PM传感器的接口
**Input 	: None
**Output 	: None
**Use 		: USART3_PM_Init();
*********************************************************************/
void USART3_PM_Init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);													
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;																		
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;													
  GPIO_Init(GPIOB, &GPIO_InitStructure);																			
  
	USART_InitStructure.USART_BaudRate = bound;																		
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											
	USART_InitStructure.USART_StopBits = USART_StopBits_1;											
	USART_InitStructure.USART_Parity = USART_Parity_No;															
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	
	USART_InitStructure.USART_Mode = USART_Mode_Rx;																

  USART_Init(USART3, &USART_InitStructure); 
  USART_Cmd(USART3, ENABLE);                 
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
}
/********************************************************************
**Function: USART3_PM_DMA_Init
**Note 		: 
**Input 	: None
**Output 	: None
**Use 		: USART3_PM_DMA_Init();
*********************************************************************/
void USART3_PM_DMA_Init(void)
{
  NVIC_InitTypeDef nvic;
	DMA_InitTypeDef dma;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel3);
  dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
  dma.DMA_MemoryBaseAddr = (uint32_t)buffer_usart3;
  dma.DMA_DIR = DMA_DIR_PeripheralSRC;
  dma.DMA_BufferSize = 20;
  dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  dma.DMA_Mode = DMA_Mode_Circular;
  dma.DMA_Priority = DMA_Priority_VeryHigh;
	dma.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel3,&dma);
	

  DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);

  DMA_Cmd(DMA1_Channel3,ENABLE); 

  nvic.NVIC_IRQChannel = DMA1_Channel3_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 7;
  nvic.NVIC_IRQChannelSubPriority = 0;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
}
/********************************************************************
**Function: DMA1_Channel3_IRQHandler
**Note 		: 
**Input 	: None
**Output 	: None
**Use 		: DMA1_Channel3_IRQHandler();
*********************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC3))
  {
		DMA_ClearFlag(DMA1_IT_TC3);
		DMA_ClearITPendingBit(DMA1_IT_TC3);
		size_t xBytesSent;
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xBytesSent = xMessageBufferSendFromISR( BufferFromDma ,
												(void * ) buffer_usart3[0],
												sizeof(buffer_usart3),
												&xHigherPriorityTaskWoken);
		if(xBytesSent != sizeof(buffer_usart3))
		{
			printf("there was not enough free space in the buffer !");
		}
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		if(buffer_usart3[0]==0xAA && buffer_usart3[1]==0xC0)			
//		{
//			int u;
//			for(u = 0;u < BUFFER_LENTH;u++) buffer_inorder[u]=buffer_usart3[u];
//			packet_dec();
//		}else
//		{
//			int i,j,k,bit;
//			if(buffer_temp[BUFFER_LENTH-1]==0xAA&&buffer_usart3[0]==0xAA)	
//			{
//				bit = BUFFER_LENTH-1;
//			}else 
//			{
//				for(i = 0;i < BUFFER_LENTH;i++)										
//				{
//						if(buffer_temp[i]==0xAA && buffer_temp[i+1]==0xAA) bit = i;		
//				}
//			}
//			for(j = 0;j < BUFFER_LENTH-bit;j++)										
//			{																											
//				buffer_inorder[j]=buffer_temp[j+bit];
//			}
//			for(k = 0;k < bit;k++)														 	
//			{																											
//				buffer_inorder[k+BUFFER_LENTH-bit]=buffer_usart3[k];
//			}
//			for(j = 0;j <BUFFER_LENTH;j++)												
//			{
//				buffer_temp[j]=buffer_usart3[j];
//			}
//			packet_dec();
//		}
//		STM32_LED_PM = !STM32_LED_PM;
	}
}
/********************************************************************
**Function: packet_dec
**Note 		: 
**Input 	: None
**Output 	: None
**Use 		: packet_dec();
*********************************************************************/
void packet_dec(void)
{
	u16 sum = 0;
	for (int i = 2;i <= 7;i++)
	{
		sum+=buffer_inorder[i];
	}
	if((sum&0xff) & buffer_inorder[8])
	{
		Pm2  = ((buffer_inorder[3]<<8)+buffer_inorder[2])/10;//  ug/m3
		Pm10 = ((buffer_inorder[5]<<8)+buffer_inorder[4])/10;
	}
}

