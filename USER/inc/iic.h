#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f4xx.h"

#define IIC_SCL  		PBout(8)
#define IIC_SDA_OUT PBout(9)
#define IIC_SDA_IN  PBin(9)

 
#define ACK    0
#define NO_ACK 1

void IIC_PinInit(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_McuReceiveAck(void);
void IIC_McuSendAck(u8 ack);
u8 IIC_McuSendByte(u8 sendData);
u8 IIC_McuReceiveByte(u8 sAck);



#endif


