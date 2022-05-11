#include "includes.h"

/***********************************************************
函数功能：IIC管脚初始化
函数形参：None
函数返回值：None
备注：
	*PA2--SCL ---推挽输出
	*PA1--SDA----开漏输出
作者：Yao
日期：2021年01月25日
版本：V0.0
***********************************************************/
void HP6_IIC_PinInit(void)
{
	GPIO_InitTypeDef GPIOA_InitStruct;
	
	//打开时钟 AHB1
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE );
	
	//初始化PA2  推挽输出
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出功能
	GPIOA_InitStruct.GPIO_OType = GPIO_OType_PP ;//推挽
	GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIOA_InitStruct.GPIO_Speed = GPIO_Low_Speed;//2Mhz
	GPIO_Init (GPIOA, &GPIOA_InitStruct);
	
	//PA1 开漏输出
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出功能
	GPIOA_InitStruct.GPIO_OType = GPIO_OType_OD ;//开漏
	GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init (GPIOA, &GPIOA_InitStruct);
	
	//初始输出高电平
	GPIO_SetBits (GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

/*********************************************
函数功能：IIC起始条件
函数形参：None
函数返回值：None
备注：*以100KHZ为例
			*在时钟线高电平期间，数据线产生下降沿  
日期：2020年11月09日
作者：Yao
版本：V0.0
*********************************************/
void HP6_IIC_Start(void)
{
	HP6_SDA_OUT = 1;
	HP6_SCL = 1;
	Delay_us(IIC_10us);
	HP6_SDA_OUT = 0;//起始条件 
	Delay_us(IIC_10us);
	HP6_SCL = 0;//组成一个完整周期
}

/*********************************************
函数功能：IIC停止条件
函数形参：None
函数返回值：None
备注：*以100KHZ为例
			*在时钟线高电平期间，数据线产生上升沿  
日期：2020年11月09日
作者：Yao
版本：V0.0
*********************************************/
void HP6_IIC_Stop(void)
{
	HP6_SDA_OUT = 0;//在时钟线之前拉低数据线，防止在数据线之后拉低会产生起始条件
	
	HP6_SCL = 1;
	
	Delay_us(IIC_10us);
	HP6_SDA_OUT = 1;//停止条件：数据线产生上升沿
	Delay_us(IIC_10us);
}

/*********************************************
函数功能：主机（单片机）发送应答信号
函数形参：ack ： 是否应答
函数返回值：None
备注：*以100KHZ为例
			*0：应答  1：非应答
日期：2020年11月09日
作者：Yao
版本：V0.0
*********************************************/
void HP6_IIC_McuSendAck(u8 ack) // 1  0
{
	HP6_SCL = 0;//发送方-主机准备应答信号给到数据线
	
	if(ack) //条件为真  ack = 1(不应答)，为假  ack = 0
		HP6_SDA_OUT = NO_ACK;
	else
		HP6_SDA_OUT = ACK;
	Delay_us(IIC_10us);//让主机给的应答信号稳定在数据线上
	
	HP6_SCL = 1;//主机控制时钟线产生上升沿 ， 接收方-从机读取应答信号
	Delay_us(IIC_10us);//给时间从机读取主机应答信号
	
	HP6_SCL = 0;//保证周期完整性
}

/*********************************************
函数功能：主机（单片机）接收一个应答信号
函数形参：None
函数返回值： 是否应答
备注：*以100KHZ为例
			*0：应答  1：非应答
日期：2020年11月09日
作者：Yao
版本：V0.0
*********************************************/
u8 HP6_IIC_McuReceiveAck(void)
{
	u8 ret_ack;
	
	HP6_SDA_OUT = 1;//断开输出电路
	
	HP6_SCL = 0;//发送方-从机准备应答信号给到数据线
	
	Delay_us(IIC_10us);//让从机给的应答信号稳定在数据线上
	
	HP6_SCL = 1;//主机控制时钟线产生上升沿，接收方-主机读取应答信号
	
	//判断输入数据寄存器 -IDR 的值
	if( HP6_SDA_IN )  //为真  == 1 
		ret_ack = NO_ACK;
	else
		ret_ack = ACK;
	
	Delay_us(IIC_10us);//给时间从机读取主机应答信号
	
	HP6_SCL = 0;
	
	return ret_ack;
}

/*********************************************
函数功能：主机（单片机）发送一个字节函数
函数形参：待发送一个字节数据
函数返回值： 从机是否应答
备注：*以100KHZ为例
			*0：应答  1：非应答
			主机作为发送方，从机作为接收方
日期：2020年11月09日
作者：Yao
版本：V0.0
*********************************************/
u8 HP6_IIC_McuSendByte(u8 send_data)
{
	u8 ret_ack;
	u8 i;
	
	for(i = 0; i < 8; i++)
	{
		HP6_SCL = 0;//主机准备数据
		
		if(send_data & (0x80 >> i)) // 1000 0000
			HP6_SDA_OUT = 1;
		else	
			HP6_SDA_OUT = 0;
		
		Delay_us(IIC_10us);//让数据稳定在数据线上
		
		HP6_SCL = 1;//时钟线产生上升沿，接收方-从机读取数据
		Delay_us(IIC_10us);//给时间从机采集数据
	}
	
	HP6_SCL = 0;
	
	ret_ack = HP6_IIC_McuReceiveAck( );//读取从机的应答信号
	
	return ret_ack;
}

/*********************************************
函数功能：主机（单片机）接收一个字节函数
函数形参：主机决定是否应答
函数返回值： 接收到的一个字节数据
备注：*以100KHZ为例
			*0：应答  1：非应答
			主机作为接收方，从机作为发送方
			主机发送应答信号
日期：2020年11月09日
作者：Yao
版本：V0.0
*********************************************/
u8 HP6_IIC_McuReceiveByte(u8 ack)
{
	u8 recv_data;
	u8 i;
	
	HP6_SDA_OUT = 1;//断开输出电路
	
	for(i = 0; i < 8; i++)
	{
		HP6_SCL = 0;//下降沿从机准备数据
		Delay_us(IIC_10us);//让数据稳定在数据线上
		
		HP6_SCL = 1;//时钟线产生上升沿，接收方-主机读取数据
		
		recv_data <<= 1;//空出最低位接收一位数据
		recv_data |= HP6_SDA_IN; //读取输入数据寄存器第9位的值保存到变量的最低位
		
		Delay_us(IIC_10us);//给时间主机采集数据
	}
	
	HP6_SCL = 0;
	
	HP6_IIC_McuSendAck(ack);//主机决定是否应答
	
	return recv_data;//返回接收到的一个字节数据
}

/*********************************************
函数功能：IIC连续写字节函数
函数形参：addr:从机地址
					t_buf:待发送的数据
					size:发送size字节
函数返回值： 接收到的size个字节数据
备注：*以100KHZ为例
			*0：发送成功  1：发送失败
日期：2020年12月01日
作者：Yao
版本：V0.0
*********************************************/
u8 HP6_IIC_McuSendData(u8 addr, u8 *t_buf, u8 size)
{
	u8 i, ack;
	
	HP6_IIC_Start( );//起始条件
	
	ack = HP6_IIC_McuSendByte(addr << 1);//器件地址+写
	if(ack)
			return 1;
	
	for(i = 0; i < size; i++)
	{
		ack = HP6_IIC_McuSendByte(*t_buf++);
		if(ack)
			return 2;
	}
	
	HP6_IIC_Stop( );//停止条件

	return 0;
}

/*********************************************
函数功能：IIC连续接收字节函数
函数形参：addr:从机地址
					t_buf:待发送的数据
					size:发送size字节
函数返回值： 接收到的size个字节数据
备注：*以100KHZ为例
			*0：接收成功  其他：接收失败
			主机作为接收方，从机作为发送方
			主机发送应答信号
日期：2020年12月01日
作者：Yao
版本：V0.0
*********************************************/
u8 HP6_IIC_McuReadData(u8 addr, u8 *r_buf, u8 size)
{
	u8 i, ack;
	
	HP6_IIC_Start( );//起始条件
	
	ack = HP6_IIC_McuSendByte((addr << 1)|1);//器件地址+读
	if(ack)
			return 3;
	
	for(i = 0; i < size-1; i++)
	{
		*r_buf++ = HP6_IIC_McuReceiveByte(ACK);
	}
	
	*r_buf = HP6_IIC_McuReceiveByte(NO_ACK);
	
	HP6_IIC_Stop( );//停止条件
	
	return 0;
}
