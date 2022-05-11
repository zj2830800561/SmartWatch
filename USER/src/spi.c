#include "includes.h"

/*******************************************
函数功能：SPI控制器初始化
函数形参：None
函数返回值：None
备注：OLED_SCL --- PB3  复用功能  AF5
			OLED_MOSI -- PB5	复用功能  AF5
			OLED_CS		-- PB7  推挽输出	
日期：2021年03月05日
作者：Yao
版本：V0.0
*******************************************/
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIOB_InitStruct;
	SPI_InitTypeDef SPI1_InitStruct;
	
	//打开GPIOB时钟
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB, ENABLE );
	
	//初始化PB7 推挽输出
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出功能
	GPIOB_InitStruct.GPIO_OType = GPIO_OType_PP ;//推挽
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIOB_InitStruct.GPIO_Speed = GPIO_Medium_Speed;//中速  10Mhz
	GPIO_Init (GPIOB, &GPIOB_InitStruct);
	
	//初始化PB3/PB5  复用功能  AF5
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_AF ;//复用功能
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_Init (GPIOB, &GPIOB_InitStruct);
	
	//映射
	GPIO_PinAFConfig (GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig (GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	//打开SPI1控制器时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SPI1, ENABLE );
	
	//初始化SPI1控制器  --  MODE3  高位在前
	SPI1_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//fPCLK/2
	SPI1_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;//后沿采集数据
	SPI1_InitStruct.SPI_CPOL = SPI_CPOL_High;//空闲电平为高电平
	SPI1_InitStruct.SPI_DataSize = SPI_DataSize_8b;//8bit
	SPI1_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//两根数据线  全双工  -- 4线SPI
	SPI1_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//高位在前
	SPI1_InitStruct.SPI_Mode = SPI_Mode_Master;//MCU作为主机
	SPI1_InitStruct.SPI_NSS = SPI_NSS_Soft;//使能软件从器件管理
	SPI_Init (SPI1, &SPI1_InitStruct);
	
	//使能SPI1
	SPI_Cmd (SPI1, ENABLE );
}


/*******************************************
函数功能：SPI控制器发送 1 个字节函数
函数形参：u8 sData: 待发送的一个字节数据
函数返回值：None
备注：发送数据：等待发送缓冲区为空	
			接收数据：等待接收缓冲去非空
日期：2021年03月05日
作者：Yao
版本：V0.0
*******************************************/
void SPI1_SendByte(u8 sData)
{
	//等待发送缓冲区为空
	while(SPI_I2S_GetFlagStatus (SPI1, SPI_I2S_FLAG_TXE) == RESET);
	
	SPI_I2S_SendData (SPI1, sData);//发送数据
	
	//等待接收缓冲区非空
	while(SPI_I2S_GetFlagStatus (SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
	//sData = SPI_I2S_ReceiveData (SPI1);//接收数据
}


