#ifndef __PTC_H
#define __PTC_H

#include "bmp180.h"
#include "ads1115.h"
#include "sht31.h"
#include "bsp.h"
#include "nbiot.h"
extern u8 Tansfer_Buffer[30];
extern uint8_t RegistrationRequestStatus;
extern u32 count;
														
void PTC_Init(void);

#endif
