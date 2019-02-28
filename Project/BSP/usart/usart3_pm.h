#ifndef __USART3_PM_H
#define __USART3_PM_H
#include "sys.h"

#define BUFFER_LENTH 10

extern u8 buffer_usart3[BUFFER_LENTH];

void USART3_PM_Init(u32 bound);
void USART3_PM_DMA_Init(void);

#endif


