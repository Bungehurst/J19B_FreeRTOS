#ifndef _IWDOG_H  
#define _IWDOG_H  
#include "stm32f10x_iwdg.h"  
void IWDG_Init(u8 prer,u16 rlr);//IWDG初始化  
void IWDG_Feed(void);  //喂狗函数
#endif 

