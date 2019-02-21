/**
  ******************************************************************************
  * @file    ADS1115.h 
  * @author  ZTY
  * @version V1.0.0
  * @date    25-October-2018
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
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

#ifndef __ADS1115_H_
#define __ADS1115_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "iic_ADS.h"

/* Exported constants --------------------------------------------------------*/
#define DEVADDR_GND  0 // I2C设备地址 GND
#define DEVADDR_VDD  1 // I2C设备地址 VDD
#define DEVADDR_SDA  2 // I2C设备地址 SDA
#define DEVADDR_SCL  3 // I2C设备地址 SCL

#define CVSNREG  0 // Conversion register 的地址
#define CONFREG  1 // Config register 的地址
#define LOREG    2 // Lo_thresh register 的地址
#define HIREG    3 // Hi_thresh register 的地址

#define CONFREG_INIT  ((u16)0x4380) // Config register 的初始化值
#define LOREG_INIT    ((u16)0x0000) // Lo_thresh register 的初始化值
#define HIREG_INIT    ((u16)0xFFFF) // Hi_thresh register 的初始化值

/* Exported macro ------------------------------------------------------------*/
#define CVSNREG_RESET          ((u16)0x0000) // Conversion register 的默认值
#define CONFREG_RESET          ((u16)0x8583) // Config register 的默认值
#define LOREG_RESET            ((u16)0x8000) // Lo_thresh register 的默认值
#define HIREG_RESET            ((u16)0x7FFF) // Hi_thresh register 的默认值

#define GASADS1115_RDYPIN      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) // GasADS1115RDY管脚
#define WINDADS1115_RDYPIN     GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) // WindADS1115RDY管脚

/* Private variables ---------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
u16 ADS1115_ReadFromRegister(u8 devAddr, u8 regAddr);
void ADS1115_WriteToRegister(u8 devAddr, u8 regAddr, u16 regValue);
void ADS1115_ALERTRDYpin_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
u8 ADS1115_Init(
	u8 devAddr,
	u16 confRegValue, 
	u16 loRegValue, 
	u16 hiRegValue,
	GPIO_TypeDef *GPIOx, 
	uint16_t GPIO_Pin
);
u8 GasADS1115_Init(
	u16 confRegValue, 
	u16 loRegValue, 
	u16 hiRegValue
);
u16 GasADS1115_ReadFromRegister(u8 regAddr);
void GasADS1115_WriteToRegister(u8 regAddr, u16 regValue);
u8 WindADS1115_Init(
	u16 confRegValue, 
	u16 loRegValue, 
	u16 hiRegValue
);
u16 WindADS1115_ReadFromRegister(u8 regAddr);
void WindADS1115_WriteToRegister(u8 regAddr, u16 regValue);

/* Private functions ---------------------------------------------------------*/


#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
