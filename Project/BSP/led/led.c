/**
  ******************************************************************************
  * @file    led.c 
  * @author  DPL
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  ******************************************************************************
  */  
/* Includes ------------------------------------------------------------------*/
#include "led.h"
/********************************************************************
**Function: LED_Init
**Note 		: LED初始化
**Input 	: None
**Output 	: None
**Use 		:	LED_Init();
*********************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOE, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|						/*STM32_LED_WS*/
																GPIO_Pin_1|						/*STM32_LED_WD*/
																GPIO_Pin_2|						/*STM32_LED_CO*/
																GPIO_Pin_3|						/*STM32_LED_SO2*/
																GPIO_Pin_4|						/*STM32_LED_NO2*/
																GPIO_Pin_5|						/*STM32_LED_O3*/
																GPIO_Pin_6|						/*STM32_LED_PM*/
																GPIO_Pin_7|						/*STM32_LED_ADC1*/
																GPIO_Pin_8|						/*STM32_LED_ADC2*/
																GPIO_Pin_9|						/*STM32_LED_SDCARD*/
																GPIO_Pin_10 ;					/*STM32_LED_NBIOT*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO口速度为50MHz
	GPIO_Init(GPIOF,&GPIO_InitStructure);					 				//根据设定参数初始化
	
	GPIO_SetBits(GPIOF,	GPIO_Pin_0|\
											GPIO_Pin_1|\
											GPIO_Pin_2|\
											GPIO_Pin_3|\
											GPIO_Pin_4|\
											GPIO_Pin_5|\
											GPIO_Pin_6|\
											GPIO_Pin_7|\
											GPIO_Pin_8|\
											GPIO_Pin_9|\
											GPIO_Pin_10 );				
											
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO口速度为50MHz
	GPIO_Init(GPIOE,&GPIO_InitStructure);					 				//根据设定参数初始化
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
}
void Reset_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO口速度为50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);					 				//根据设定参数初始化
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	
}

