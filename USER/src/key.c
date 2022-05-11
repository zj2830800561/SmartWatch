#include "includes.h"

/*******************************************
函数功能：KEY初始化
函数形参：None
函数返回值：None
备注：PA0-KEY_OK  AHB1
日期：2021年03月03日
作者：Yao
版本：V0.0
*******************************************/
void KEY_Init(void)
{
	/*********************KEY_OK初始化**************************/
	GPIO_InitTypeDef GPIOA_InitStruct;
	
	//打开时钟 AHB1
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE );
	
	//初始化PA0
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_IN;//输入功能
	GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIOA_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init (GPIOA, &GPIOA_InitStruct);
	
	/*********************KEY_ADIN初始化**************************/
	Adc1_Init( );
}


/*******************************************
函数功能：按键扫描函数
函数形参：None
函数返回值：按下：返回键码  没有：返回0
备注：PA0-KEY_OK  AHB1   按下是高电平
日期：2021年03月03日
作者：Yao
版本：V0.0
*******************************************/
u8 Key_Scan(void)
{
	static u8 flag = 0;
	
	if(keyAD[0] > 4000)
	{
		return KeY_LEFT;
	}else if(keyAD[0] > 2000)
	{
		return Key_RIGHT;
	}else if(keyAD[0] > 1300)
	{
		return Key_DOWN;
	}else if(keyAD[0] > 1000)
	{
		return Key_UP;
	}
	
	if((KEY_OK == 1) && (flag == 0))
	{
		//延时消抖 10ms
		
		if(KEY_OK == 1)//按下
		{
			flag = 1;			
			return Key_OK;
		}
	}else if((KEY_OK == 0) && (flag == 1))//松手
	{
		flag = 0;
	}
	
	return Key_NO;
}

