#ifndef __NBIoT_H_
#define __NBIoT_H_
#include "usart2_send.h"
#include "led.h"
#include "hexstring.h"
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
/***************************************************************
* 函数名称: NB_CONNRCT_STATUS
* 说    明: 定义NB连接状态的结构体，供调用查看
* 参    数: 无
* 返 回 值: 无
***************************************************************/
typedef struct
{
	uint8_t		NB_ATTRACT_NET;				//NB附着网络
}NB_CONNRCT_STATUS;
/***************************************************************
* 函数名称: NB_CONNRCT_STATUS
* 说    明: 定义NB连接状态的结构体，供调用查看
* 参    数: 无
* 返 回 值: 无
***************************************************************/
typedef struct
{
	uint8_t ref;
	uint8_t objId;
	uint8_t insCount;

	uint8_t discoverMsgId_3303[8];
	uint8_t discoverMsgId_3304[8];
	uint8_t discoverMsgId_3323[8];
	uint8_t discoverMsgId_3315[8];
	uint8_t discoverMsgId_3316[8];

	
	uint8_t observeMsgId_3303_0[8];
	uint8_t observeMsgId_3304_0[8];
	uint8_t observeMsgId_3323_0[8];
	
	uint8_t observeMsgId_3315_0[8];
	uint8_t observeMsgId_3315_1[8];
	uint8_t observeMsgId_3315_2[8];
	uint8_t observeMsgId_3315_3[8];
	
	uint8_t observeMsgId_3316_0[8];
	uint8_t observeMsgId_3316_1[8];
	uint8_t observeMsgId_3316_2[8];
	uint8_t observeMsgId_3316_3[8];
	
} ONENET_DEVICE;

extern int err_stat;
extern ONENET_DEVICE oneNetDevice;
//-----------------------宏定义-------------------------------// 
#define DefaultTimeout	1000				//默认超时时间1s

/**************************************************************/
/**************         NB接口        ***********************/
/**************************************************************/
int NB_SendCmd(uint8_t *cmd,uint8_t *result,uint32_t timeOut,uint8_t isPrintf);
int NB_SendCmd_Mul(uint8_t *cmd,uint8_t *result1,uint8_t *result2,uint32_t timeOut,uint8_t isPrintf);
void NB_InitConnect(void);
void NB_SendMsgToServer(uint8_t *msg);
void NB_SetCDPServer(uint8_t *ncdpIP,uint8_t *port);
uint8_t NB_ReceiveMsg(uint8_t *msgReceive);
/**************************************************************/
/**************         NB对接oneNet接口        ***************/
/**************************************************************/
void NB_InitConnectOneNet(void);
uint8_t NB_SendRegistrationRequest(void);
void NB_ResourcesRegister_Single(char *discoverMsgId);
void NB_ResourcesRegister_Multiple(char *discoverMsgId);
void NB_NotifyOneNetMsg(const char *observeMsgId,uint8_t *objectId,uint8_t *insid,uint8_t *resId,uint8_t *msg);
void NB_DeleteRegistrationRequest(void);
uint8_t NB_SendRegistrationRequest_2(void);




#endif
