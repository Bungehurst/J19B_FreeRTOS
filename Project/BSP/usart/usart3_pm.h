#ifndef __USART3_PM_H
#define __USART3_PM_H

#include "bsp.h"
#include "ptc.h"
#include "app.h"

void USART3_PM_Init(u32 bound);
void packet_dec(void);
void USART3_PM_DMA_Init(void);

#endif


