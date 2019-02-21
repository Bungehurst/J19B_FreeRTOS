/**
  ******************************************************************************
  * @file    timer.c 
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
#include "timer.h"
/********************************************************************
**Function: TIMER_Init
**Note 		: 定时器5初始化
**Input 	: u16 arr	重装载值
						u16 psc 预分频系数
**Output 	: None
**Use 		:	TIMER_Init();
*********************************************************************/
void TIMER_Init(u16 arr,u16 psc)
{	 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);						//使能TIM5时钟

	//初始化定时器5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; 												//设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 											//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 				//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	 	//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 								//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 								//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  			//先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  						//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 								//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  																//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);												//允许更新中断
	
	TIM_Cmd(TIM2,ENABLE ); 													//使能定时器5

}

/********************************************************************
**Function: TIM2_IRQHandler
**Note 		: 定时器5中断服务程序	
**Input 	: None
**Output 	: None
**Use 		: 
*********************************************************************/
void TIM2_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 			//清除中断标志位
		IWDG_Feed();
		count++;
	}
}

