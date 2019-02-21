		//文件头 防止重复定义
#ifndef __MYIIC_ADS_H
#define __MYIIC_ADS_H

		//包含文件部分
#include "sys.h"
#include "delay.h"

		//宏定义部分

//IO方向设置 若改变IO口则需改变下面的配置
#define SDA_ADS_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_ADS_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 
#define IIC_ADS_SCL    PBout(6) //SCL
#define IIC_ADS_SDA    PBout(7) //SDA	 
#define READ_SDA_ADS   PBin(7)  //输入SDA 


		//函数声明部分

//IIC所有操作函数
void IIC_ADS_Init(void);                //初始化IIC的IO口				 
void IIC_ADS_Start(void);				//发送IIC开始信号
void IIC_ADS_Stop(void);	  			//发送IIC停止信号
void IIC_ADS_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_ADS_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_ADS_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_ADS_Ack(void);					//IIC发送ACK信号
void IIC_ADS_NAck(void);				//IIC不发送ACK信号

#endif
