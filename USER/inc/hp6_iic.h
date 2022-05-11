#ifndef _HP6_IIC_H_
#define _HP6_IIC_H_

#include "stm32f4xx.h"

#define HP6_SCL     PAout(2)   
#define HP6_SDA_IN  PAin(1)   
#define HP6_SDA_OUT PAout(1)

#define IIC_100khz  4   //100khz
#define IIC_10us    10	//HP-6延时时长不能少于10us 

#define ACK    0
#define NO_ACK 1

/************HP6************/
void HP6_IIC_PinInit(void);
void HP6_IIC_Start(void);
void HP6_IIC_Stop(void);
u8 HP6_IIC_McuReceiveAck(void);
void HP6_IIC_McuSendAck(u8 ack);
u8 HP6_IIC_McuSendByte(u8 sendData);
u8 HP6_IIC_McuReceiveByte(u8 sAck);

u8 HP6_IIC_McuSendData(u8 addr, u8 *sBuf, u8 num);
u8 HP6_IIC_McuReadData(u8 addr, u8 *rBuf, u8 num);

#endif


