#include "includes.h"


/*******************************************
函数功能：OLED管脚初始化
函数形参：None
函数返回值：None
备注：OLED_SCL --- PB3  复用功能  AF5
			OLED_MOSI -- PB5	复用功能  AF5
			OLED_CS		-- PB7  推挽输出	
			OLED_DC 	-- PA15 推挽输出
			OLED_RES	-- PB13	推挽输出
日期：2021年03月05日
作者：Yao
版本：V0.0
*******************************************/
void OLED_PinInit(void)
{
	GPIO_InitTypeDef GPIOB_InitStruct;
	
	/******************SPI管脚初始化*************************/
	SPI1_Init( );
	
	/******************OLED管脚初始化*************************/
	//打开GPIOB/GPIOA时钟
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE );
	
	//初始化PB13 推挽输出
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出功能
	GPIOB_InitStruct.GPIO_OType = GPIO_OType_PP ;//推挽
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIOB_InitStruct.GPIO_Speed = GPIO_Medium_Speed;//中速  10Mhz
	GPIO_Init (GPIOB, &GPIOB_InitStruct);
	
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init (GPIOA, &GPIOB_InitStruct);//PA15
}

/*******************************************
函数功能：OLED写一个字节函数
函数形参：
				*u8 sData:待发送的一个字节数据
				*u8 cmd:数据命令选择
函数返回值：None
备注：DC = 0:发送命令
			DC = 1:发送数据
日期：2021年03月05日
作者：Yao
版本：V0.0
*******************************************/
void OLED_WriteByte(u8 sData, u8 cmd)
{	
	OLED_CS = 0;//拉低片选
	
	OLED_DC = cmd;
	
	SPI1_SendByte(sData);//发送一个字节数据/命令
	
	OLED_CS = 1;//拉高片选
}

/*******************************************
函数功能：OLED设置显示位置函数
函数形参：
				*u8 page:页地址
				*u8 col:列地址
函数返回值：None
备注：页地址：0xBx
			列低地址：0x0x  列高地址：0x1x
			BUG:该OLED屏前两列无法正常显示
日期：2021年03月05日
作者：Yao
版本：V0.0
*******************************************/
void OLED_SetPos(u8 page, u8 col)
{
	col += 2;//列地址偏移两列  第1、2列无法正常显示
	
	OLED_WriteByte(0xB0 | page, OLED_CMD);//设置页地址
	OLED_WriteByte(0x00 | (col & 0x0f), OLED_CMD);//设置列地址低四位
	OLED_WriteByte(0x10 | ((col & 0xf0)>>4), OLED_CMD);//设置列地址高四位
}

/*******************************************
函数功能：OLED清屏函数
函数形参：None
函数返回值：None
备注：130*64
			BUG:该OLED屏前两列无法正常显示
日期：2021年03月05日
作者：Yao
版本：V0.0
*******************************************/
void OLED_Clear(void)
{
	u8 i, j;
	
	for(i = 0; i < 8; i++)//控制显示页数
	{
		//设置显示位置
		OLED_SetPos(i, 0);
		
		//发送显示内容
		for(j = 0; j < 130; j++)//控制显示列数
		{
			OLED_WriteByte(0x00, OLED_DATA);//发送显示数据
		}
	}
}


/*******************************************
函数功能：OLED初始化序列函数
函数形参：None
函数返回值：None
备注：None
日期：2021年03月05日
作者：Yao
版本：V0.0
*******************************************/
void OLED_Init(void)
{ 
	OLED_PinInit();	//管脚初始化
	
	//复位OLED
	OLED_RES = 0;
	Delay_ms(100);
	OLED_RES = 1;
	
	OLED_WriteByte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WriteByte(0x02,OLED_CMD);//---SET low column address
	OLED_WriteByte(0x10,OLED_CMD);//---SET high column address
	OLED_WriteByte(0x40,OLED_CMD);//--SET start line address  SET Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteByte(0x81,OLED_CMD);//--SET contrast control register
	OLED_WriteByte(0xCF,OLED_CMD); // SET SEG Output Current Brightness
	OLED_WriteByte(0xA1,OLED_CMD);//--SET SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WriteByte(0xC8,OLED_CMD);//SET COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WriteByte(0xA6,OLED_CMD);//--SET normal display
	OLED_WriteByte(0xA8,OLED_CMD);//--SET multiplex ratio(1 to 64)
	OLED_WriteByte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WriteByte(0xD3,OLED_CMD);//-SET display offSET	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteByte(0x00,OLED_CMD);//-not offSET
	OLED_WriteByte(0xd5,OLED_CMD);//--SET display clock divide ratio/oscillator frequency
	OLED_WriteByte(0x80,OLED_CMD);//--SET divide ratio, SET Clock as 100 Frames/Sec
	OLED_WriteByte(0xD9,OLED_CMD);//--SET pre-charge period
	OLED_WriteByte(0xF1,OLED_CMD);//SET Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WriteByte(0xDA,OLED_CMD);//--SET com pins hardware configuration
	OLED_WriteByte(0x12,OLED_CMD);
	OLED_WriteByte(0xDB,OLED_CMD);//--SET vcomh
	OLED_WriteByte(0x40,OLED_CMD);//SET VCOM Deselect Level
	OLED_WriteByte(0x20,OLED_CMD);//-SET Page Addressing Mode (0x00/0x01/0x02)
	OLED_WriteByte(0x02,OLED_CMD);//
	OLED_WriteByte(0x8D,OLED_CMD);//--SET Charge Pump enable/disable
	OLED_WriteByte(0x14,OLED_CMD);//--SET(0x10) disable
	OLED_WriteByte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WriteByte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WriteByte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WriteByte(0xAF,OLED_CMD); /*display ON*/ 
		
	OLED_Clear();
}  


/*******************************************
函数功能：OLED显示任意函数
函数形参：显示位置，显示内容，字体大小，对应大小的字库
函数返回值：None
备注：16*8  16   32*16   32
日期：2021年03月05日
作者：Yao
版本：V0.0
*******************************************/
void OLED_DisAscii(u8 page, u8 col, u8 ch, u8 size, u8 *font)
{
	u8 i, j;
	u8 pos;
	
	pos = ch - ' ';	
	
	for(i = 0; i < size/8; i++)//控制显示页数
	{
		//设置显示的初始位置
		OLED_SetPos(page+i, col);
		
		for(j = 0; j < size/2; j++)//控制显示列数  for(j = 0; j < 8; j++)
		{
			OLED_WriteByte(font[pos*(size*size/16) + j + i*(size/2)], OLED_DATA);//发送显示的数据
			//size*size/16  ---- 一个size字符对应的字模数据个数
		}
	}
}


/*******************************************
函数功能：OLED显示任意汉字字符函数
函数形参：显示位置，显示内容，字体大小，位置字库，对应大小的字库
函数返回值：None
备注：16*16  16   32*32   32
日期：2021年03月05日
作者：Yao
版本：V0.0
*******************************************/
void OLED_DisChinese(u8 page, u8 col, u8 *hz, u8 size, u8 *font_m)
{
	u8 i, j;
	u8 pos = 0;
	
	for(i = 0; i < (strlen((const char *)chinese_font)/2); i++)//遍历汉字字库寻找汉字
	{
		if((hz[0]==chinese_font[i*2]) && (hz[1]==chinese_font[i*2+1]))
		{
			pos = i;//找到汉字的位置
			break;
		}
	}
	
	if(pos != i)//没找到
	{
		printf("display error\r\n");
		return ;
	}		
	
	for(i = 0; i < size/8; i++)//控制显示页数
	{
		//设置显示的初始位置
		OLED_SetPos(page+i, col);
		
		for(j = 0; j < size; j++)//控制显示列数  for(j = 0; j < 8; j++)
		{
			OLED_WriteByte(font_m[pos*(size*size/8) + j + i*size], OLED_DATA);//发送显示的数据
			//size*size/8 ---- 一个size字符对应的字模数据个数
		}
	}
}


/*******************************************
函数功能：OLED显示字符串函数
函数形参：显示位置，显示内容，字体大小，对应大小的字库
函数返回值：None
备注：16*8  16   32*16   32
日期：2021年03月05日
作者：Yao
版本：V0.0
*******************************************/
void OLED_DisString(u8 page, u8 col, u8 *str, u8 size, u8 *font_e, u8 *font_c)
{
	while(*str != '\0')
	{
		//判断是什么字符
		if(*str < 128) //英文字符
		{
			OLED_DisAscii(page, col, *str, size, font_e);//显示英文字符
			
			str++;
			col += size/2;
		}else if((*str > 0xa0) && (*(str+1) > 0xa0)) //汉字字符
		{
			OLED_DisChinese(page, col, str, size, font_c);//显示汉字字符
			
			str += 2;
			col += size;
		}
		
		if(*str < 128)
		{
			if(col > (130 - size/2))
			{
				page += size/8;//换页
				col = 0;//列地址重置
			}
		}else if((*str > 0xa0) && (*(str+1) > 0xa0))
		{
			if(col > (130 - size))
			{
				page += size/8;//换页
				col = 0;//列地址重置
			}
		}
	}
}


