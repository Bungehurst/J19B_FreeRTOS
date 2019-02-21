/**
  ******************************************************************************
  * @file    ADS1115.c 
  * @author  ZTY
  * @version V2.0.0
  * @date    17-November-2018
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

/* Includes ------------------------------------------------------------------*/
#include "ads1115.h"
/* constants -----------------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  读取ADS1115的寄存器
  * @param  u8 devAddr 要读取的ADS1115的地址
			取值：0 GND
				  1 VDD
				  2 SDA
				  3 SCL
			u8 regAddr 要读取的寄存器的地址
			取值：0 Conversion register 存储转换完成后数值的寄存器
				  1 Config register
				  2 Lo_thresh register
				  3 Hi_thresh register
  * @retval u16 寄存器的值
  */
u16 ADS1115_ReadFromRegister(u8 devAddr, u8 regAddr)
{	
	u16 regValue = 0; // 寄存器数据
	
	IIC_ADS_Start();
	IIC_ADS_Send_Byte(0x90|(devAddr<<1)); // 发送从机地址
	IIC_ADS_Wait_Ack();                   // 等待应答
	IIC_ADS_Send_Byte(0x00|regAddr);      // 发送寄存器地址
	IIC_ADS_Wait_Ack();                   // 等待应答
	IIC_ADS_Start();
	IIC_ADS_Send_Byte(0x91|(devAddr<<1)); // 发送从机地址
	IIC_ADS_Wait_Ack();                   // 等待应答
	regValue = (IIC_ADS_Read_Byte(1)<<8); // 读取高八位数据并发送应答
	regValue |= IIC_ADS_Read_Byte(1);     // 读取低八位数据并发送应答
	IIC_ADS_Stop();
	delay_us(3);                      // 总线空闲时间
	
	return regValue;
}


/**
  * @brief  写ADS1115的寄存器
  * @param  u8 devAddr 要写入的ADS1115的地址
			取值：0 GND
				  1 VDD
				  2 SDA
				  3 SCL
			u8 regAddr 要写入的寄存器的地址
			取值：0 Conversion register 存储转换完成后数值的寄存器
				  1 Config register
				  2 Lo_thresh register
				  3 Hi_thresh register
			u16 RegValue 写入寄存器的值
  * @retval None
  */
void ADS1115_WriteToRegister(u8 devAddr, u8 regAddr, u16 regValue)
{	
	IIC_ADS_Start();
	IIC_ADS_Send_Byte(0x90|(devAddr<<1)); // 发送从机地址
	IIC_ADS_Wait_Ack();                   // 等待应答
	IIC_ADS_Send_Byte(0x00|regAddr);      // 发送寄存器地址
	IIC_ADS_Wait_Ack();                   // 等待应答
	IIC_ADS_Send_Byte((u8)(regValue>>8)); // 发送高八位数据
	IIC_ADS_Wait_Ack();                   // 等待应答
	IIC_ADS_Send_Byte((u8)(regValue));    // 发送低八位数据
	IIC_ADS_Wait_Ack();                   // 等待应答
	IIC_ADS_Stop();
	delay_us(3);                      // 总线空闲时间
}


/**
  * @brief  ADS1115的ALERT/RDY管脚初始化
  * @param  GPIO_TypeDef *GPIOx GPIO组号 GPIOA-GPIOG
			uint16_t GPIO_Pin GPIO管脚号 GPIO_Pin_0-GPIO_Pin_15
  * @retval None
  */
void ADS1115_ALERTRDYpin_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	switch((u32)GPIOx)
	{
		case (u32)GPIOA:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟
			break;
		}
		case (u32)GPIOB:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟
			break;
		}
		case (u32)GPIOC:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能GPIOC时钟
			break;
		}
		case (u32)GPIOD:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//使能GPIOD时钟
			break;
		}
		case (u32)GPIOE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	//使能GPIOE时钟
			break;
		}
		case (u32)GPIOF:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	//使能GPIOF时钟
			break;
		}
		case (u32)GPIOG:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	//使能GPIOG时钟
			break;
		}
		default: break;
	}
	   
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;   // 浮空输入
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}


/**
  * @brief  ADS1115初始化
  * @param  u8 devAddr 要写入的ADS1115的地址
			取值：0 GND
				  1 VDD
				  2 SDA
				  3 SCL
			u16 confRegValue 写入 Config register 的值
			u16 loRegValue 写入 Lo_thresh register 的值
			u16 hiRegValue 写入 Hi_thresh register 的值
			GPIO_TypeDef *GPIOx GPIO组号 取值：GPIOA-GPIOG
			uint16_t GPIO_Pin GPIO管脚号 取值：GPIO_Pin_0-GPIO_Pin_15
  * @retval u8 配置结果
			取值：0 配置成功
				  1 配置出错
  */
u8 ADS1115_Init(
	u8 devAddr,
	u16 confRegValue, 
	u16 loRegValue, 
	u16 hiRegValue,
	GPIO_TypeDef *GPIOx, 
	uint16_t GPIO_Pin
)
{
	u8 i = 2; // 计数变量
	
	IIC_ADS_Init(); // 初始化I2C IO口 PB6 SCL  PB7 SDA
	ADS1115_ALERTRDYpin_Init(GPIOx, GPIO_Pin); // 初始化ALERT/RDY管脚
	
	// 如果读出的寄存器值与上电默认值不符，则报错退出
	if((ADS1115_ReadFromRegister(devAddr, CVSNREG) != CVSNREG_RESET) ||
		(ADS1115_ReadFromRegister(devAddr, CONFREG) != CONFREG_RESET) ||
		(ADS1115_ReadFromRegister(devAddr, LOREG) != LOREG_RESET) ||
		(ADS1115_ReadFromRegister(devAddr, HIREG) != HIREG_RESET))
	{
		return 1;
	}
	
	// 配置内部寄存器 若3次配置失败则报错
	do
	{
		ADS1115_WriteToRegister(devAddr, CONFREG, confRegValue);
		ADS1115_WriteToRegister(devAddr, LOREG, loRegValue);
		ADS1115_WriteToRegister(devAddr, HIREG, hiRegValue);

		if((ADS1115_ReadFromRegister(devAddr, CONFREG) == (confRegValue|0x8000)) && // Config register 最高位写入和读取的含义不一样
		(ADS1115_ReadFromRegister(devAddr, LOREG) == loRegValue) &&
		(ADS1115_ReadFromRegister(devAddr, HIREG) == hiRegValue))
		{
			return 0;
		}
	}
	while(i--);
	
	return 1;
}


/**
  * @brief  GasADS1115初始化 芯片标号U1
  * @param  u16 confRegValue 写入 Config register 的值
			u16 loRegValue 写入 Lo_thresh register 的值
			u16 hiRegValue 写入 Hi_thresh register 的值
  * @retval u8 配置结果
			取值：0 配置成功
				  1 配置出错
  */
u8 GasADS1115_Init(
	u16 confRegValue, 
	u16 loRegValue, 
	u16 hiRegValue
)
{
	return ADS1115_Init(DEVADDR_VDD, confRegValue, loRegValue, hiRegValue, GPIOC, GPIO_Pin_3);
}


/**
  * @brief  读取GasADS1115的寄存器
  * @param  u8 regAddr 要读取的寄存器的地址
			取值：0 Conversion register 存储转换完成后数值的寄存器
				  1 Config register
				  2 Lo_thresh register
				  3 Hi_thresh register
  * @retval u16 寄存器的值
  */
u16 GasADS1115_ReadFromRegister(u8 regAddr)
{	
	return ADS1115_ReadFromRegister(DEVADDR_VDD, regAddr);
}


/**
  * @brief  写GasADS1115的寄存器
  * @param  u8 regAddr 要写入的寄存器的地址
			取值：0 Conversion register 存储转换完成后数值的寄存器
				  1 Config register
				  2 Lo_thresh register
				  3 Hi_thresh register
			u16 RegValue 写入寄存器的值
  * @retval None
  */
void GasADS1115_WriteToRegister(u8 regAddr, u16 regValue)
{
	ADS1115_WriteToRegister(DEVADDR_VDD, regAddr, regValue);
}


/**
  * @brief  WindADS1115初始化 芯片标号U2
  * @param  u16 confRegValue 写入 Config register 的值
			u16 loRegValue 写入 Lo_thresh register 的值
			u16 hiRegValue 写入 Hi_thresh register 的值
  * @retval u8 配置结果
			取值：0 配置成功
				  1 配置出错
  */
u8 WindADS1115_Init(
	u16 confRegValue, 
	u16 loRegValue, 
	u16 hiRegValue
)
{
	return ADS1115_Init(DEVADDR_GND, confRegValue, loRegValue, hiRegValue, GPIOC, GPIO_Pin_4);
}


/**
  * @brief  读取WindADS1115的寄存器
  * @param  u8 regAddr 要读取的寄存器的地址
			取值：0 Conversion register 存储转换完成后数值的寄存器
				  1 Config register
				  2 Lo_thresh register
				  3 Hi_thresh register
  * @retval u16 寄存器的值
  */
u16 WindADS1115_ReadFromRegister(u8 regAddr)
{	
	return ADS1115_ReadFromRegister(DEVADDR_GND, regAddr);
}


/**
  * @brief  写WindADS1115的寄存器
  * @param  u8 regAddr 要写入的寄存器的地址
			取值：0 Conversion register 存储转换完成后数值的寄存器
				  1 Config register
				  2 Lo_thresh register
				  3 Hi_thresh register
			u16 RegValue 写入寄存器的值
  * @retval None
  */
void WindADS1115_WriteToRegister(u8 regAddr, u16 regValue)
{
	ADS1115_WriteToRegister(DEVADDR_GND, regAddr, regValue);
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
