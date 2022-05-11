#include "includes.h"


/***********************************************************
函数功能：IIC管脚初始化
函数形参：None
函数返回值：None
备注：
	*PB8--SCL ---推挽输出
	*PB9--SDA----开漏输出
作者：Yao
日期：2021年01月25日
版本：V0.0
***********************************************************/
void IIC_PinInit(void)
{
	GPIO_InitTypeDef GPIOB_InitStruct;
	
	//打开时钟 AHB1
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB, ENABLE );
	
	//初始化PB8  推挽输出
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出功能
	GPIOB_InitStruct.GPIO_OType = GPIO_OType_PP ;//推挽
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIOB_InitStruct.GPIO_Speed = GPIO_Low_Speed;//2Mhz
	GPIO_Init (GPIOB, &GPIOB_InitStruct);
	
	//PB9 开漏输出
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出功能
	GPIOB_InitStruct.GPIO_OType = GPIO_OType_OD ;//开漏
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init (GPIOB, &GPIOB_InitStruct);
	
	//初始输出高电平
	GPIO_SetBits (GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
}

/***********************************************************
函数功能：IIC延时函数
函数形参：u8 t
函数返回值：None
备注：
作者：Yao
日期：2021年01月25日
版本：V0.0
***********************************************************/
void IIC_Delay(void)
{
	Delay_us(4);//100khz
	//Delay_us(1);//400Khz
}

/***********************************************************
函数功能：IIC起始条件
函数形参：None
函数返回值：None
备注：时钟线在高电平期间，数据线产生下降沿
作者：Yao
日期：2021年01月25日
版本：V0.0
***********************************************************/
void IIC_Start(void)
{
	IIC_SDA_OUT = 1;
	IIC_SCL = 1;
	
	IIC_Delay( );//起始条件的建立时间
	IIC_SDA_OUT = 0;
	
	IIC_Delay( );//起始条件的保持时间
	IIC_SCL = 0;//
}


/***********************************************************
函数功能：IIC停止条件
函数形参：None
函数返回值：None
备注：时钟线在高电平期间，数据线产生上升沿
作者：Yao
日期：2021年01月25日
版本：V0.0
***********************************************************/
void IIC_Stop(void)
{
	IIC_SDA_OUT = 0;
	IIC_SCL = 1;
	
	IIC_Delay( );//停止条件的建立时间
	IIC_SDA_OUT = 1;//停止条件：数据线产生上升沿
	
	IIC_Delay( );//起始条件的保持时间
}

/***********************************************************
函数功能：主机（MCU）接收一个应答信号
函数形参：None
函数返回值：应答信号
备注：
	*时钟线产生下架沿：发送方准备数据
	*时钟线产生上升沿：接收方采集数据
	*接收方：MCU   发送方：AT24C02
作者：Yao
日期：2021年01月25日
版本：V0.0
***********************************************************/
u8 IIC_McuReceiveAck(void)
{
	u8 ack;
	
	IIC_SDA_OUT = 1;//断开输出电路  输入正常工作 
	
	IIC_SCL = 0;//下降沿  从机准备应答信号
	
	IIC_Delay( );//延时4us   
	
	IIC_SCL = 1;//上升沿  MCU读取应答信号
	if(IIC_SDA_IN)
		ack = 1;//非应答
	else
		ack = 0;//应答
	IIC_Delay( );//延时4us;//
	
	IIC_SCL = 0;//保持周期完整性
	//IIC_Delay( );//延时4us;

	return ack;
}

/***********************************************************
函数功能：主机（MCU）发送一个应答信号
函数形参：应答信号
函数返回值：None
备注：
	*时钟线产生下架沿：发送方准备数据
	*时钟线产生上升沿：接收方采集数据
	*接收方：AT24C02   发送方：MCU
作者：Yao
日期：2021年01月25日
版本：V0.0
***********************************************************/
void IIC_McuSendAck(u8 ack)
{
	IIC_SCL = 0;//下降沿 发送方(MCU)准备数据
	
	IIC_SDA_OUT = ack;
	IIC_Delay( );//延时4us;//让应答信号稳定在数据线上
	
	IIC_SCL = 1;//上升沿  从机采集应答数据
	IIC_Delay( );//延时4us;//给时间从机接收应答
	
	IIC_SCL = 0;//保持周期的完整性
	//IIC_Delay( );//延时4us;
}

/***********************************************************
函数功能：主机（MCU）发送一个字节数据并读取应答
函数形参：sendData
函数返回值：从机是否应答
备注：
	*时钟线产生下架沿：发送方准备数据
	*时钟线产生上升沿：接收方采集数据
	*接收方：AT24C02   发送方：MCU
	*高位在前
作者：Yao
日期：2021年01月25日
版本：V0.0
***********************************************************/
u8 IIC_McuSendByte(u8 sendData)
{
	u8 i, rAck;
	
	for(i = 0; i < 8; i++)
	{
		IIC_SCL = 0;//发送方准备数据
		
		if(sendData & (0x80 >> i))//1000 0000
			IIC_SDA_OUT = 1;
		else
			IIC_SDA_OUT = 0;
		
		IIC_Delay( );//延时4us;//让数据稳定在数据线上
		
		IIC_SCL = 1;//产生上升沿  接收方采集数据
		
		IIC_Delay( );//从机采集数据时间
	}
	
	IIC_SCL = 0;
	rAck = IIC_McuReceiveAck( );
	
	return rAck;
}


/***********************************************************
函数功能：主机（MCU）读取一个字节数据并发送应答信号
函数形参：主机的应答信号
函数返回值：读取到的一个字节数据
备注：
	*时钟线产生下架沿：发送方准备数据
	*时钟线产生上升沿：接收方采集数据
	*接收方：MCU   发送方：AT24C02
	*高位在前
作者：Yao
日期：2021年01月25日
版本：V0.0
***********************************************************/
u8 IIC_McuReceiveByte(u8 sAck)
{
	u8 revData = 0, i;
	
	IIC_SDA_OUT = 1;//断开输出电路
	
	for(i = 0; i < 8; i++)
	{
		IIC_SCL = 0;//下降沿  发送方（从机）准备数据
		
		IIC_Delay( );//从机准备数据时间
		
		IIC_SCL = 1;//上升沿  接收方（MCU）采集数据
		
		revData <<= 1;//空出最低位接收数据
		revData |= IIC_SDA_IN;
		
		IIC_Delay( );//主机采集数据时间
	}
	
	IIC_SCL = 0;
	IIC_McuSendAck(sAck);
	
	return revData;
}




