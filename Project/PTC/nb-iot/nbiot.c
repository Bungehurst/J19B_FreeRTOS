#include "nbiot.h"
static char cmdSend[512];
ONENET_DEVICE oneNetDevice;
int err_stat = 0;
/***************************************************************
* 函数名称: NB_SendCmd
* 说    明: NB发送数据
* 参    数: uint8_t *cmd，需要发送的命令
*						uint8_t *result，期望获得的结果
*						uint32_t timeOut，等待期望结果的时间
*						uint8_t isPrintf，是否打印Log
* 返 回 值: ret，返回值
***************************************************************/
int NB_SendCmd(uint8_t *cmd,uint8_t *result,uint32_t timeOut,uint8_t isPrintf)
{
	int ret=0;
	u32 count_1 = 0;
	printf("%s",cmd);
	delay_ms(500);
	count_1 = count;
	while(1)
	{
		if(strstr((const char *)USART2_RX_BUF,(const char *)result)!=NULL)
		{
			ret = 0;
			break;
		}
		else
		{
			printf("%s",cmd);
			delay_ms(timeOut);
		}
		if((count - count_1)>10) break;
	}	
	delay_ms(timeOut);
	return ret;
}

/***************************************************************
* 函数名称: NB_SendCmd_Mul
* 说    明: NB发送数据，返回值有多个
* 参    数: uint8_t *cmd，需要发送的命令
*						uint8_t *result，期望获得的结果1
*						uint8_t *result，期望获得的结果2
*						uint32_t timeOut，等待期望结果的时间
*						uint8_t isPrintf，是否打印Log
* 返 回 值: ret，返回值
***************************************************************/
int NB_SendCmd_Mul(uint8_t *cmd,uint8_t *result1,uint8_t *result2,uint32_t timeOut,uint8_t isPrintf)
{
	int ret=0;
	u32 count_1 = 0;
	printf("%s",cmd);
	delay_ms(100);
	count_1 = count;
	while(1)
	{
		if(strstr((const char *)USART2_RX_BUF,(const char *)result1) 
			||strstr((const char *)USART2_RX_BUF,(const char *)result2))
		{
			ret = 0;
			break;
		}
		else
		{
			printf("%s",cmd);
			delay_ms(timeOut);
		}
		if((count - count_1)>10) break;
	}
	delay_ms(timeOut);
	return ret;
}

/***************************************************************
* 函数名称: NB_InitConnectOneNet
* 说    明: NB初始化入网过程
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void NB_InitConnectOneNet(void)
{
	NB_SendCmd((uint8_t *)"AT+CGATT?\r\n",(uint8_t *)"+CGATT:1",DefaultTimeout,1);
	NB_SendCmd((uint8_t *)"AT+CEREG=1\r\n",(uint8_t *)"OK",DefaultTimeout,1);
	NB_SendCmd((uint8_t *)"AT+CMEE=1\r\n",(uint8_t *)"OK",DefaultTimeout,1);
	NB_SendCmd((uint8_t *)"AT+CEREG?\r\n",(uint8_t *)"+CEREG:1,1",DefaultTimeout,1);
}

/***************************************************************
* 函数名称: NB_Create_OneNET_Instance
* 说    明: NB发送注册请求
* 参    数: *msg，要发送的数据（String形式）
* 返 回 值: 无
***************************************************************/
void NB_Create_OneNET_Instance()
{
}


/***************************************************************
* 函数名称: NB_SendRegistrationRequest
* 说    明: NB发送注册请求  初始化版
* 参    数: 无
* 返 回 值: 注册状态
*						1，注册成功
*						0，注册失败
***************************************************************/
uint8_t NB_SendRegistrationRequest(void)
{
	//创建OneNet通信实例
	NB_SendCmd((uint8_t *)"AT+MIPLCREATE\r\n",(uint8_t *)"OK",DefaultTimeout,1);
	//添加LWM2M对象
	NB_SendCmd((uint8_t *)"AT+MIPLADDOBJ=0,3303,1,\"1\",1,1\r\n",(uint8_t *)"OK",DefaultTimeout,1);
	NB_SendCmd((uint8_t *)"AT+MIPLADDOBJ=0,3304,1,\"1\",1,1\r\n",(uint8_t *)"OK",DefaultTimeout,1);
	NB_SendCmd((uint8_t *)"AT+MIPLADDOBJ=0,3315,4,\"1111\",1,1\r\n",(uint8_t *)"OK",DefaultTimeout,1);
	NB_SendCmd((uint8_t *)"AT+MIPLADDOBJ=0,3316,4,\"1111\",1,1\r\n",(uint8_t *)"OK",DefaultTimeout,1);
	NB_SendCmd((uint8_t *)"AT+MIPLADDOBJ=0,3323,1,\"1\",1,1\r\n",(uint8_t *)"OK",DefaultTimeout,1);
		//NB向OneNet发送注册请求
	int ret=0;
	int stat = 3315;
	int i_1 = 0;
	u32 timeOutCount_2 = 0;
	u32 timeOutCount = 0;
	static char *discoverMsgIdPos1 = NULL,*observeMsgIdPos1= NULL;
	static char *discoverMsgIdPos2[5] = {NULL,NULL,NULL,NULL,NULL},*observeMsgIdPos2[11]= {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
	printf((const char*)"AT+MIPLOPEN=0,9600,60\r\n");
	for(int i = 0;i<10;i++) delay_ms(1000);
	timeOutCount = count;
	while(1)
	{
		observeMsgIdPos1 = strstr((const char *)USART2_RX_BUF,(const char *)"+MIPLOBSERVE: 0,");
		if(observeMsgIdPos1)
		{
			if(ret == 1)
				break;
			delay_ms(1000);delay_ms(1000);delay_ms(1000);
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[0] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3303,0");
				if(observeMsgIdPos2[0])
				{
					for(i_1 = 0;*(observeMsgIdPos2[0]-9+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3303_0,(observeMsgIdPos2[0]-8+i_1),(7-i_1));
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}	
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[1] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3304,0");
				if(observeMsgIdPos2[1])
				{
					for(i_1 = 0;*(observeMsgIdPos2[1]-9+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3304_0,(observeMsgIdPos2[1]-8+i_1),(7-i_1));
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}	
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[3] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3315,0");
				if(observeMsgIdPos2[3])
				{
					for(i_1 = 0;*(observeMsgIdPos2[3]-9+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3315_0,(observeMsgIdPos2[3]-8+i_1),(7-i_1));
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}	
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[4] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3315,1");
				if(observeMsgIdPos2[4])
				{
					for(i_1 = 0;*(observeMsgIdPos2[4]-9+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3315_1,(observeMsgIdPos2[4]-8+i_1),(7-i_1));
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}	
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[5] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3315,2");
				if(observeMsgIdPos2[5])
				{
					for(i_1 = 0;*(observeMsgIdPos2[5]-9+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3315_2,(observeMsgIdPos2[5]-8+i_1),(7-i_1));
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}	
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[6] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3315,3");
				if(observeMsgIdPos2[6])
				{
					for(i_1 = 0;*(observeMsgIdPos2[6]-9+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3315_3,(observeMsgIdPos2[6]-8+i_1),(7-i_1));
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}	
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[7] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3316,0");
				if(observeMsgIdPos2[7])
				{
					for(i_1 = 0;*(observeMsgIdPos2[7]-9+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3316_0,(observeMsgIdPos2[7]-8+i_1),(7-i_1));
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}	
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[8] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3316,1");
				if(observeMsgIdPos2[8])
				{
					for(i_1 = 0;*(observeMsgIdPos2[8]-9+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3316_1,(observeMsgIdPos2[8]-8+i_1),(7-i_1));
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}	
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[9] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3316,2");
				if(observeMsgIdPos2[9])
				{
					for(i_1 = 0;*(observeMsgIdPos2[9]-9+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3316_2,(observeMsgIdPos2[9]-8+i_1),(7-i_1));
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}	
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[10] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3316,3");
				if(observeMsgIdPos2[10])
				{
					for(i_1 = 0;*(observeMsgIdPos2[10]-9+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3316_3,(observeMsgIdPos2[10]-8+i_1),(7-i_1));
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}	
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[2] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3323,0");
				if(observeMsgIdPos2[2])
				{
					for(i_1 = 0;*(observeMsgIdPos2[2]-9+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3323_0,(observeMsgIdPos2[2]-8+i_1),(7-i_1));
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}			
			}
			ret = 1;
		}
		if(ret == 1)
		{
			break;
		}
				
		if((count - timeOutCount)>90)
		{
			timeOutCount = 0;
			ret = 0;
			break;
		}			
		delay_ms(100);

	}
	
	memset(USART2_RX_BUF,1,((u8 *)observeMsgIdPos2[2]-USART2_RX_BUF+15));
	
	timeOutCount = count;

	while(ret == 1)
	{
		discoverMsgIdPos1 = strstr((const char *)USART2_RX_BUF,(const char *)"+MIPLDISCOVER: 0,");
		if(discoverMsgIdPos1)
		{
			if(ret == 2)
				break;
			switch(stat)
			{
				case 3315:
					timeOutCount_2 = count;
					while(1)
					{
						discoverMsgIdPos2[0] = strstr((const char *)USART2_RX_BUF,(const char *)",3315");
						if(discoverMsgIdPos2[0])
						{
							for(i_1 = 0;*(discoverMsgIdPos2[0]-9+i_1)!=',';i_1++);
							memcpy(oneNetDevice.discoverMsgId_3315,discoverMsgIdPos2[0]-9+i_1+1,discoverMsgIdPos2[0]-(discoverMsgIdPos2[0]-9+i_1+1));	
							NB_ResourcesRegister_Multiple((char *)oneNetDevice.discoverMsgId_3315);
							break;
						}
						if((count - timeOutCount_2)>8) 
						{
							timeOutCount = 0;
							ret = 0;
							break;
						}
					}
					stat = 3316;
					break;
				case 3316:
					timeOutCount_2 = count;
					while(1)
					{
						discoverMsgIdPos2[1] = strstr((const char *)USART2_RX_BUF,(const char *)",3316");
						if(discoverMsgIdPos2[1])
						{
							for(i_1 = 0;*(discoverMsgIdPos2[1]-9+i_1)!=',';i_1++);
							memcpy(oneNetDevice.discoverMsgId_3316,discoverMsgIdPos2[1]-9+i_1+1,discoverMsgIdPos2[1]-(discoverMsgIdPos2[1]-9+i_1+1));	
							NB_ResourcesRegister_Multiple((char *)oneNetDevice.discoverMsgId_3316);
							break;
						}
						if((count - timeOutCount_2)>8) 
						{
							timeOutCount = 0;
							ret = 0;
							break;
						}
					}
					stat = 3303;
					break;
				case 3303:
					timeOutCount_2 = count;
					while(1)
					{
						discoverMsgIdPos2[2] = strstr((const char *)USART2_RX_BUF,(const char *)",3303");
						if(discoverMsgIdPos2[2])
						{
							for(i_1 = 0;*(discoverMsgIdPos2[2]-9+i_1)!=',';i_1++);
							memcpy(oneNetDevice.discoverMsgId_3303,discoverMsgIdPos2[2]-9+i_1+1,discoverMsgIdPos2[2]-(discoverMsgIdPos2[2]-9+i_1+1));	
							NB_ResourcesRegister_Single((char *)oneNetDevice.discoverMsgId_3303);
							break;
						}
						if((count - timeOutCount_2)>8) 
						{
							timeOutCount = 0;
							ret = 0;
							break;
						}
					}
					stat = 3304;
					break;
				case 3304:
					timeOutCount_2 = count;
					while(1)
					{			
						discoverMsgIdPos2[3] = strstr((char *)USART2_RX_BUF,(const char *)",3304");
						if(discoverMsgIdPos2[3])
						{
							for(i_1 = 0;*(discoverMsgIdPos2[3]-9+i_1)!=',';i_1++);
							memcpy(oneNetDevice.discoverMsgId_3304,discoverMsgIdPos2[3]-9+i_1+1,discoverMsgIdPos2[3]-(discoverMsgIdPos2[3]-9+i_1+1));		
							NB_ResourcesRegister_Single((char *)oneNetDevice.discoverMsgId_3304);
							break;
						}
						if((count - timeOutCount_2)>8) 
						{
							timeOutCount = 0;
							ret = 0;
							break;
						}
					}
					stat = 3323;
					break;
				case 3323:
					timeOutCount_2 = count;
					while(1)
					{	
						discoverMsgIdPos2[4] = strstr((char *)USART2_RX_BUF,(const char *)",3323");
						if(discoverMsgIdPos2[4])
						{
							for(i_1 = 0;*(discoverMsgIdPos2[4]-9+i_1)!=',';i_1++);
							memcpy(oneNetDevice.discoverMsgId_3323,discoverMsgIdPos2[4]-9+i_1+1,discoverMsgIdPos2[4]-(discoverMsgIdPos2[4]-9+i_1+1));	
							NB_ResourcesRegister_Single((char *)oneNetDevice.discoverMsgId_3323);
							break;
						}
						if((count - timeOutCount_2)>8) 
						{
							timeOutCount = 0;
							ret = 0;
							break;
						}
					}
					ret = 2;
					break;
				default:break;
			}
		}
		if(ret == 2)
			break;
		if((count - timeOutCount)>45) 
		{
			timeOutCount = 0;
			ret = 0;
			break;
		}
		delay_ms(100);
	}
	return ret;
}

/***************************************************************
* 函数名称: NB_ResourcesRegister_Single
* 说    明: NB发送注册请求  单属性
* 参    数: *msg，要发送的数据（String形式）
* 返 回 值: 无
***************************************************************/
void NB_ResourcesRegister_Single(char *discoverMsgId)
{
	memset(cmdSend,0,sizeof(cmdSend));
	strcat(cmdSend,"AT+MIPLDISCOVERRSP=0,");
	strcat(cmdSend,(const char *)discoverMsgId);
	strcat(cmdSend,",");
	strcat(cmdSend,"1,4,\"5700\"");
	strcat(cmdSend,"\r\n");
	NB_SendCmd((uint8_t *)cmdSend,(uint8_t *)"OK",DefaultTimeout,1);	
}
/***************************************************************
* 函数名称: NB_ResourcesRegister_Single
* 说    明: NB发送注册请求  四属性
* 参    数: *msg，要发送的数据（String形式）
* 返 回 值: 无
***************************************************************/
void NB_ResourcesRegister_Multiple(char *discoverMsgId)
{
	memset(cmdSend,0,sizeof(cmdSend));
	strcat(cmdSend,"AT+MIPLDISCOVERRSP=0,");
	strcat(cmdSend,(const char *)discoverMsgId);
	strcat(cmdSend,",");
	strcat(cmdSend,"1,19,\"5700;5700;5700;5700\"");
	strcat(cmdSend,"\r\n");
	NB_SendCmd((uint8_t *)cmdSend,(uint8_t *)"OK",DefaultTimeout,1);	
}
/***************************************************************
* 函数名称: NB_NotifyOneNetMsg
* 说    明: NB向OneNet发送数据
* 参    数: *resId，要发送的数据（String形式）
*						*msg，要发送的数据（String形式）
* 返 回 值: 无
***************************************************************/
void NB_NotifyOneNetMsg(const char *observeMsgId,uint8_t *objectId,uint8_t *insid,uint8_t *resId,uint8_t *msg)
{
	u32 count_1 = 0;
	
	memset(cmdSend,0,sizeof(cmdSend));
	strcat(cmdSend,"AT+MIPLNOTIFY=0,");
	strcat(cmdSend,(const char *)observeMsgId);
	strcat(cmdSend,",");
	strcat(cmdSend,(const char*)objectId);
	strcat(cmdSend,",");
	strcat(cmdSend,(const char*)insid);
	strcat(cmdSend,",");
	if(!strcmp((const char *)resId,"5700"))
		strcat(cmdSend,"5700,4,4,");
	strcat(cmdSend,(const char *)msg);
	strcat(cmdSend,",0,0");
	strcat(cmdSend,"\r\n");
	
	memset(USART2_RX_BUF,1,sizeof(USART2_RX_BUF));
	printf("%s",cmdSend);
	delay_ms(500);
	count_1 = count;
	while(1)
	{
		if(strstr((const char *)USART2_RX_BUF,(const char *)"OK")!=NULL)
		{
			break;
		}
		else
		{
			printf("%s",cmdSend);
			delay_ms(1000);
		}
		if((count-count_1)>5) 
		{
			err_stat = 1;
			break;
		}
		
	}	
	delay_ms(1000);
}

/***************************************************************
* 函数名称: NB_DeleteRegistrationRequest
* 说    明: NB删除入网的配置
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void NB_DeleteRegistrationRequest(void)
{
	NB_SendCmd_Mul((uint8_t *)"AT+MIPLCLOSE=0\r\n",(uint8_t *)"OK",(uint8_t *)"ERROR",DefaultTimeout,1);
	
	NB_SendCmd_Mul((uint8_t *)"AT+MIPLDELOBJ=0,3303\r\n",(uint8_t *)"OK",(uint8_t *)"ERROR",DefaultTimeout,1);
	
	NB_SendCmd_Mul((uint8_t *)"AT+MIPLDELOBJ=0,3304\r\n",(uint8_t *)"OK",(uint8_t *)"ERROR",DefaultTimeout,1);
	
	NB_SendCmd_Mul((uint8_t *)"AT+MIPLDELOBJ=0,3323\r\n",(uint8_t *)"OK",(uint8_t *)"ERROR",DefaultTimeout,1);
	
	NB_SendCmd_Mul((uint8_t *)"AT+MIPLDELOBJ=0,3315\r\n",(uint8_t *)"OK",(uint8_t *)"ERROR",DefaultTimeout,1);
	
	NB_SendCmd_Mul((uint8_t *)"AT+MIPLDELOBJ=0,3316\r\n",(uint8_t *)"OK",(uint8_t *)"ERROR",DefaultTimeout,1);

	NB_SendCmd_Mul((uint8_t *)"AT+MIPLDELETE=0\r\n",(uint8_t *)"OK",(uint8_t *)"ERROR",DefaultTimeout,1);
}

/***************************************************************
* 函数名称: NB_SendRegistrationRequest_2
* 说    明: NB发送注册请求  APP 版
* 参    数: 无
* 返 回 值: 注册状态
*						1，注册成功
*						0，注册失败
***************************************************************/
uint8_t NB_SendRegistrationRequest_2(void)
{
	//NB向OneNet发送注册请求
	int ret=0;
	int stat = 3315;
	int i_1 = 0;
	u32 timeOutCount = 0;
	u32 timeOutCount_2 = 0;
	static char *discoverMsgIdPos1 = NULL,*observeMsgIdPos1= NULL;
	static char *discoverMsgIdPos2[5] = {NULL,NULL,NULL,NULL,NULL},*observeMsgIdPos2[11]= {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
	printf((const char*)"AT+MIPLOPEN=0,9600,60\r\n");
	for(int i = 0;i<10;i++) delay_ms(1000);
	timeOutCount = count;
	while(1)
	{
		observeMsgIdPos1 = strstr((const char *)USART2_RX_BUF,(const char *)"+MIPLOBSERVE: 0,");
		if(observeMsgIdPos1)
		{
			if(ret == 1)
				break;
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[0] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3303,0");
				if(observeMsgIdPos2[0])
				{
					for(i_1 = 0;*(observeMsgIdPos2[0]-12+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3303_0,observeMsgIdPos2[0]-12+i_1+1,observeMsgIdPos2[0]-(observeMsgIdPos2[0]-12+i_1)-2);
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[1] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3304,0");
				if(observeMsgIdPos2[1])
				{
					for(i_1 = 0;*(observeMsgIdPos2[1]-12+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3304_0,observeMsgIdPos2[1]-12+i_1+1,observeMsgIdPos2[1]-(observeMsgIdPos2[1]-12+i_1)-2);
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[3] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3315,0");
				if(observeMsgIdPos2[3])
				{
					for(i_1 = 0;*(observeMsgIdPos2[3]-12+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3315_0,observeMsgIdPos2[3]-12+i_1+1,observeMsgIdPos2[3]-(observeMsgIdPos2[3]-12+i_1)-2);
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[4] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3315,1");
				if(observeMsgIdPos2[4])
				{
					for(i_1 = 0;*(observeMsgIdPos2[4]-12+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3315_1,observeMsgIdPos2[4]-12+i_1+1,observeMsgIdPos2[4]-(observeMsgIdPos2[4]-12+i_1)-2);
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[5] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3315,2");
				if(observeMsgIdPos2[5])
				{
					for(i_1 = 0;*(observeMsgIdPos2[5]-12+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3315_2,observeMsgIdPos2[5]-12+i_1+1,observeMsgIdPos2[5]-(observeMsgIdPos2[5]-12+i_1)-2);
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[6] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3315,3");
				if(observeMsgIdPos2[6])
				{
					for(i_1 = 0;*(observeMsgIdPos2[6]-12+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3315_3,observeMsgIdPos2[6]-12+i_1+1,observeMsgIdPos2[6]-(observeMsgIdPos2[6]-12+i_1)-2);
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[7] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3316,0");
				if(observeMsgIdPos2[7])
				{
					for(i_1 = 0;*(observeMsgIdPos2[7]-12+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3316_0,observeMsgIdPos2[7]-12+i_1+1,observeMsgIdPos2[7]-(observeMsgIdPos2[7]-12+i_1)-2);
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[8] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3316,1");
				if(observeMsgIdPos2[8])
				{
					for(i_1 = 0;*(observeMsgIdPos2[8]-12+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3316_1,observeMsgIdPos2[8]-12+i_1+1,observeMsgIdPos2[8]-(observeMsgIdPos2[8]-12+i_1)-2);
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[9] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3316,2");
				if(observeMsgIdPos2[9])
				{
					for(i_1 = 0;*(observeMsgIdPos2[9]-12+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3316_2,observeMsgIdPos2[9]-12+i_1+1,observeMsgIdPos2[9]-(observeMsgIdPos2[9]-12+i_1)-2);
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[10] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3316,3");
				if(observeMsgIdPos2[10])
				{
					for(i_1 = 0;*(observeMsgIdPos2[10]-12+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3316_3,observeMsgIdPos2[10]-12+i_1+1,observeMsgIdPos2[10]-(observeMsgIdPos2[10]-12+i_1)-2);
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}
			}
			timeOutCount_2 = count;
			while(1)
			{
				observeMsgIdPos2[2] = strstr((const char *)USART2_RX_BUF,(const char *)"1,3323,0");
				if(observeMsgIdPos2[2])
				{
					for(i_1 = 0;*(observeMsgIdPos2[2]-12+i_1)!=',';i_1++);
					memcpy(oneNetDevice.observeMsgId_3323_0,observeMsgIdPos2[2]-12+i_1+1,observeMsgIdPos2[2]-(observeMsgIdPos2[2]-12+i_1)-2);
					break;
				}
				if((count - timeOutCount_2)>8)
				{
					timeOutCount = 0;
					ret = 0;
					break;
				}
			}
			ret = 1;
		}
		if(ret == 1)
		{
			break;
		}
		if((count - timeOutCount)>90) 
		{
			timeOutCount = 0;
			ret = 0;
			break;
		}
		delay_ms(100);

	}
	memset(USART2_RX_BUF,1,((u8 *)observeMsgIdPos2[2]-USART2_RX_BUF+15));
	
	timeOutCount = count;

	while(ret == 1)
	{
		discoverMsgIdPos1 = strstr((const char *)USART2_RX_BUF,(const char *)"+MIPLDISCOVER: 0,");
		if(discoverMsgIdPos1)
		{
			if(ret == 2)
				break;
			switch(stat)
			{
				case 3315:
					timeOutCount_2 = count;
					while(1)
					{
						discoverMsgIdPos2[0] = strstr((const char *)USART2_RX_BUF,(const char *)",3315");
						if(discoverMsgIdPos2[0])
						{
							for(i_1 = 0;*(discoverMsgIdPos2[0]-9+i_1)!=',';i_1++);
							memcpy(oneNetDevice.discoverMsgId_3315,discoverMsgIdPos2[0]-9+i_1+1,discoverMsgIdPos2[0]-(discoverMsgIdPos2[0]-9+i_1+1));	
							NB_ResourcesRegister_Multiple((char *)oneNetDevice.discoverMsgId_3315);
							break;
						}
						if((count - timeOutCount_2)>8) 
						{
							timeOutCount = 0;
							ret = 0;
							break;
						}
					}
					stat = 3316;
					break;
				case 3316:
					timeOutCount_2 = count;
					while(1)
					{
						discoverMsgIdPos2[1] = strstr((const char *)USART2_RX_BUF,(const char *)",3316");
						if(discoverMsgIdPos2[1])
						{
							for(i_1 = 0;*(discoverMsgIdPos2[1]-9+i_1)!=',';i_1++);
							memcpy(oneNetDevice.discoverMsgId_3316,discoverMsgIdPos2[1]-9+i_1+1,discoverMsgIdPos2[1]-(discoverMsgIdPos2[1]-9+i_1+1));	
							NB_ResourcesRegister_Multiple((char *)oneNetDevice.discoverMsgId_3316);
							break;
						}
						if((count - timeOutCount_2)>8) 
						{
							timeOutCount = 0;
							ret = 0;
							break;
						}
					}
					stat = 3303;
					break;
				case 3303:
					timeOutCount_2 = count;
					while(1)
					{
						discoverMsgIdPos2[2] = strstr((const char *)USART2_RX_BUF,(const char *)",3303");
						if(discoverMsgIdPos2[2])
						{
							for(i_1 = 0;*(discoverMsgIdPos2[2]-9+i_1)!=',';i_1++);
							memcpy(oneNetDevice.discoverMsgId_3303,discoverMsgIdPos2[2]-9+i_1+1,discoverMsgIdPos2[2]-(discoverMsgIdPos2[2]-9+i_1+1));	
							NB_ResourcesRegister_Single((char *)oneNetDevice.discoverMsgId_3303);
							break;
						}
						if((count - timeOutCount_2)>8) 
						{
							timeOutCount = 0;
							ret = 0;
							break;
						}
					}
					stat = 3304;
					break;
				case 3304:
					timeOutCount_2 = count;
					while(1)
					{			
						discoverMsgIdPos2[3] = strstr((char *)USART2_RX_BUF,(const char *)",3304");
						if(discoverMsgIdPos2[3])
						{
							for(i_1 = 0;*(discoverMsgIdPos2[3]-9+i_1)!=',';i_1++);
							memcpy(oneNetDevice.discoverMsgId_3304,discoverMsgIdPos2[3]-9+i_1+1,discoverMsgIdPos2[3]-(discoverMsgIdPos2[3]-9+i_1+1));		
							NB_ResourcesRegister_Single((char *)oneNetDevice.discoverMsgId_3304);
							break;
						}
						if((count - timeOutCount_2)>8) 
						{
							timeOutCount = 0;
							ret = 0;
							break;
						}
					}
					stat = 3323;
					break;
				case 3323:
					timeOutCount_2 = count;
					while(1)
					{	
						discoverMsgIdPos2[4] = strstr((char *)USART2_RX_BUF,(const char *)",3323");
						if(discoverMsgIdPos2[4])
						{
							for(i_1 = 0;*(discoverMsgIdPos2[4]-9+i_1)!=',';i_1++);
							memcpy(oneNetDevice.discoverMsgId_3323,discoverMsgIdPos2[4]-9+i_1+1,discoverMsgIdPos2[4]-(discoverMsgIdPos2[4]-9+i_1+1));	
							NB_ResourcesRegister_Single((char *)oneNetDevice.discoverMsgId_3323);
							break;
						}
						if((count - timeOutCount_2)>8) 
						{
							timeOutCount = 0;
							ret = 0;
							break;
						}
					}
					ret = 2;
					break;
				default:break;
			}
		}
		if(ret == 2)
			break;
		if((count - timeOutCount)>45)
		{
			timeOutCount = 0;
			ret = 0;
			break;
		}
		delay_ms(100);
	}
	return ret;
}
