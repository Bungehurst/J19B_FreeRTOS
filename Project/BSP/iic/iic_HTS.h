#ifndef __IIC_HTS_H
#define __IIC_HTS_H
#include "sys.h"
#include "delay.h"
#include "led.h"
/****************
SDA----PA5
SCL----PA4
****************/

//IO方向设置 若改变IO口则需改变下面的配置
#define SDA_HTS_IN()  {GPIOA->CRL&=0XFF0FFFFF; GPIOA->CRL|=(u32)8<<20;}
#define SDA_HTS_OUT() {GPIOA->CRL&=0XFF0FFFFF; GPIOA->CRL|=(u32)3<<20;}

//IO操作函数	 
#define IIC_HTS_SCL    PAout(4) //SCL
#define IIC_HTS_SDA    PAout(5) //SDA	 
#define READ_SDA_HTS   PAin(5)  //输入SDA 

//IIC所有操作函数
void IIC_HTS_Init(void);                //初始化IIC的IO口				 
void IIC_HTS_Start(void);				//发送IIC开始信号
void IIC_HTS_Stop(void);	  			//发送IIC停止信号
void IIC_HTS_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_HTS_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_HTS_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_HTS_Ack(void);					//IIC发送ACK信号
void IIC_HTS_NAck(void);				//IIC不发送ACK信号

#endif
