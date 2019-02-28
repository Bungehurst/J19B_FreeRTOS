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
#include "OS_Task.h"
#include "includes.h"
/* Varibales -----------------------------------------------------------------*/
u8 buffer_usart3[BUFFER_LENTH];
/********************************************************************
**Function: USART3_PM_Init
**Note 		: USART3 Initialize and Configuration , Usart3 is the port of PM Sensor
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
	size_t xBytesSent;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if(DMA_GetITStatus(DMA1_IT_TC3))
  {
		DMA_ClearFlag(DMA1_IT_TC3);
		DMA_ClearITPendingBit(DMA1_IT_TC3);
		xBytesSent = xMessageBufferSendFromISR( BufferFromDma ,
												(void * ) buffer_usart3[0],
												sizeof(buffer_usart3),
												&xHigherPriorityTaskWoken);
		if(xBytesSent != sizeof(buffer_usart3))
		{
			printf("there was not enough free space in the buffer !");
		}
		memset(buffer_usart3,0x00,sizeof(buffer_usart3));
		/* Scheduler to highest priority task when quit from ISR */
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}


