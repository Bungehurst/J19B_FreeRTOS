#ifndef __SHT31_H
#define __SHT31_H

#include "iic_HTS.h"   	
#include "delay.h"

#define SHT31_ADDR 0x88  //ADDR (pin 2)connected to VDD

void SHT31_Read(char *tem ,unsigned char *hum);
void SHT31_Write(unsigned char msb,unsigned char lsb);

#endif
