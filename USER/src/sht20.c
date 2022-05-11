#include "includes.h"


/*******************************************
函数功能：SHT20软复位函数
函数形参：None
函数返回值：0:正常  其他：失败
备注：None
日期：2021年03月08日
作者：Yao
版本：V0.0
*******************************************/
u8 SHT20_SoftReset(void)
{
	u8 ack;
	
	//发送起始条件
	IIC_Start( );
	
	//发送器件地址+写
	ack = IIC_McuSendByte(ADDR_W);
	if(ack)
	{
		return 1;//非应答
	}
	
	//发送软软复位指令
	ack = IIC_McuSendByte(SOFT_RESET);
	if(ack)
	{
		return 2;//非应答
	}
	
	//发送停止条件
	IIC_Stop( );
	
	return 0;
}


/*******************************************
函数功能：SHT20初始化函数
函数形参：None
函数返回值：0:正常  其他：失败
备注：管脚 + 软复位
日期：2021年03月08日
作者：Yao
版本：V0.0
*******************************************/
u8 SHT20_Init(void)
{
	u8 ret;
	
	IIC_PinInit( );//SHT20 IIC管脚初始化
	
	//软复位
	ret = SHT20_SoftReset( );
	if(ret)
	{
		return 3;//复位失败
	}
	
	//延时15ms
	Delay_ms(RESET_TIME);
	
	return 0;
}

/*******************************************
函数功能：SHT20测量函数
函数形参：测量温度/湿度
函数返回值：返回测量数据:正常  其他：失败
备注：None
日期：2021年03月08日
作者：Yao
版本：V0.0
*******************************************/
float SHT20_MeasureCmd(u8 cmd)
{
	u8 ack;
	u16 rData;
	
	IIC_Start( );//发送起始条件
	
	ack = IIC_McuSendByte(ADDR_W);//发送器件地址+写
	if(ack)
	{
		return 4;//非应答
	}
	
	ack = IIC_McuSendByte(cmd);//发送测量命令
	if(ack)
	{
		return 5;//非应答
	}
	
	do
	{
		IIC_Start( );//发送起始条件
		ack = IIC_McuSendByte(ADDR_R);//发送器件地址+读
		Delay_ms(10);
	}while(ack);
	
	//测量完成
	rData = IIC_McuReceiveByte(ACK);//高字节
	rData <<= 8;
	rData |= IIC_McuReceiveByte(NO_ACK);//低字节
	
	IIC_Stop( );//发送停止条件
	
	rData &= ~(0x3 << 0);//清楚最低两位状态，进行换算
	
	if(cmd == TEMP_MEASURE)
	{
		return (175.72*rData/65536 - 46.85);
	}else if(cmd == HUMI_MEASURE)
	{
		return (125.0*rData/65536.0 - 6.0);
	}
	
	return 6;//测量指令有误
}

