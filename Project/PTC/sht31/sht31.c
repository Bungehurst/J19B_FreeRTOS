/**
  ******************************************************************************
  * @file    SHT31.c 
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
#include "sht31.h"

unsigned char buffer_tem[2]={0};
unsigned char buffer_hum[2]={0};

/********************************************************************
**Function: SHT31_Init
**Note 		: 温湿度传感器初始化
**Input 	: None
**Output 	: None
**Use 		: SHT31_Init();
*********************************************************************/

void SHT31_Write(unsigned char msb,unsigned char lsb)
{
  IIC_HTS_Start();                  	//起始信号
  IIC_HTS_Send_Byte(SHT31_ADDR+0);   	//发送设备地址+写信号
  IIC_HTS_Wait_Ack();									//从机应答
  IIC_HTS_Send_Byte(msb);    					//发送命令
  IIC_HTS_Wait_Ack();
  IIC_HTS_Send_Byte(lsb);    					//发送命令
  IIC_HTS_Wait_Ack();
  IIC_HTS_Stop();                   	//发送停止信号
	delay_ms(5);
}


void SHT31_Read( char *tem ,unsigned char *hum)
{
	int flag = 0;
		SHT31_Write(0x20,0x32); 							//数据采集频率1 m/s
		delay_ms(200);
		IIC_HTS_Start();   										//起始信号
		IIC_HTS_Send_Byte(SHT31_ADDR+1);      //发送设备地址+读信号
		flag = IIC_HTS_Wait_Ack();
		buffer_tem[0]=IIC_HTS_Read_Byte(1);  	//发送ACK
		buffer_tem[1]=IIC_HTS_Read_Byte(1);  	//发送ACK
	
	  IIC_HTS_Read_Byte(1); 
	
		buffer_hum[0]=IIC_HTS_Read_Byte(1);  //发送ACK
		buffer_hum[1]=IIC_HTS_Read_Byte(1);  //发送ACK
		IIC_HTS_Stop();                          //发送停止信号
		delay_ms(5);

		*tem=((unsigned long)175*(buffer_tem[0]*256+buffer_tem[1]))/65535-45; //摄氏度
		*hum=((unsigned long)100*(buffer_hum[0]*256+buffer_hum[1]))/65535;
		if(flag==1) 
		{
			*tem = 0;
			*hum = 0;
		}
		buffer_tem[0]=0;
		buffer_tem[1]=0;
		buffer_hum[0]=0;
		buffer_hum[1]=0;
		
}

