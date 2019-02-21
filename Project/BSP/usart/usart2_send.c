/**
  ******************************************************************************
  * @file    usart2_send.c 
  * @author  DPL
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  ******************************************************************************
  */  
/* Includes ------------------------------------------------------------------*/
#include "usart2_send.h"
/* Variables ------------------------------------------------------------------*/
u8 USART2_RX_BUF[USART2_REC_LEN];	//串口2接收缓存
//#pragma import(__use_no_semihosting)      
//struct __FILE 
//{ 
//	int handle; 
//}; 

//FILE __stdout;       
////定义_sys_exit()以避免使用半主机模式    
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 
////重定义fputc函数 
//int fputc(int ch, FILE *f)
//{ 	
//	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
//	USART2->DR = (u8) ch;      
//	return ch;
//}


/********************************************************************
**Function: USART2_Send_Init
**Note 		: USART2初始化及配置，USART2是无线透传模块的接口
**Input 	: None
**Output 	: None
**Use 		: USART2_Send_Init();
*********************************************************************/
void USART2_Send_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  USART_InitTypeDef usart2;
	DMA_InitTypeDef dma;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_DeInit(USART2);
	USART_StructInit(&usart2);
	usart2.USART_BaudRate = 9600;
	usart2.USART_WordLength = USART_WordLength_8b;
	usart2.USART_StopBits = USART_StopBits_1;
	usart2.USART_Parity = USART_Parity_No;
	usart2.USART_Mode = USART_Mode_Tx|USART_Mode_Rx ;
	usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART2,&usart2);
	//USART_Cmd(USART2,ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	
  DMA_DeInit(DMA1_Channel6);
  dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
  dma.DMA_MemoryBaseAddr = (uint32_t)USART2_RX_BUF;
  dma.DMA_DIR = DMA_DIR_PeripheralSRC;
  dma.DMA_BufferSize = USART2_REC_LEN;
  dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  dma.DMA_Mode = DMA_Mode_Circular;
	dma.DMA_Priority = DMA_Priority_VeryHigh;
	dma.DMA_M2M = DMA_M2M_Disable;
	
  DMA_Init(DMA1_Channel6,&dma);
	//DMA_Cmd(DMA1_Channel6,ENABLE); 


}


