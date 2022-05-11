#include "includes.h"

/*******************************************
函数功能：led初始化
函数形参：None
函数返回值：None
备注：PA7-LED
日期：2021年03月03日
作者：Yao
版本：V0.0
*******************************************/
void Led_Init(void)
{
	GPIO_InitTypeDef GPIOA_InitStruct;
	
	//打开时钟 AHB1
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE );
	
	//初始化PA7
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出功能
	GPIOA_InitStruct.GPIO_OType = GPIO_OType_PP ;//推挽
	GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIOA_InitStruct.GPIO_Speed = GPIO_Low_Speed;//2Mhz
	GPIO_Init (GPIOA, &GPIOA_InitStruct);
	
	//初始输出高电平
	GPIO_SetBits (GPIOA, GPIO_Pin_7);
}
