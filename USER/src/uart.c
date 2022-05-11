#include "includes.h"

/*******************************************
函数功能：USART1初始化函数
函数形参：波特率
函数返回值：None
备注：USART1-TX -- PA9    复用 AF7
			USART1-RX -- PA10
日期：2021年03月03日
作者：Yao
版本：V0.0
*******************************************/
void UART1_Init(u32 baud)
{
	GPIO_InitTypeDef UART1_InitStruct;
	USART_InitTypeDef USART1_InitStruct;
	
	//打开GPIOA时钟
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE );
	
	//初始化PA9\PA10复用功能 映射AF7
	UART1_InitStruct.GPIO_Mode = GPIO_Mode_AF ;//复用功能
	UART1_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init (GPIOA, &UART1_InitStruct);
	
	//映射
	GPIO_PinAFConfig (GPIOA, GPIO_PinSource9, GPIO_AF_USART1);//TX
	GPIO_PinAFConfig (GPIOA, GPIO_PinSource10, GPIO_AF_USART1);//RX
	
	//打开USART1时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE );
	
	//初始化USART1  四要素：波特率 数据位  校验位  停止位
	USART1_InitStruct.USART_BaudRate = baud;
	USART1_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART1_InitStruct.USART_Parity = USART_Parity_No;//无校验
	USART1_InitStruct.USART_StopBits = USART_StopBits_1 ;//1个停止位
	USART1_InitStruct.USART_WordLength = USART_WordLength_8b;//8bit
	USART_Init (USART1, &USART1_InitStruct);
	
	//使能USART1
	USART_Cmd (USART1, ENABLE );
}

/********************printf支持函数****************************/
#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) {
	while((USART1->SR &(0X01<<7))==0){};
		USART1->DR=ch;
  return (ch);
}
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}

void _ttywrch(int ch) {
  while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
}

void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
