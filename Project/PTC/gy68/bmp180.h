#ifndef _BMP180_H_
#define _BMP180_H_

#include "iic_HTS.h"
#include "math.h"

#define BMP180_Address 0xee

s32 BMP180_Update(void);
void BMP180_Write(u8 add, u8 da);
void BMP180_Multiple_Read(u8 add,u8*BUF,u16 num);


#endif



