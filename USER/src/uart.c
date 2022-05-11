#include "includes.h"

/*******************************************
�������ܣ�USART1��ʼ������
�����βΣ�������
��������ֵ��None
��ע��USART1-TX -- PA9    ���� AF7
			USART1-RX -- PA10
���ڣ�2021��03��03��
���ߣ�Yao
�汾��V0.0
*******************************************/
void UART1_Init(u32 baud)
{
	GPIO_InitTypeDef UART1_InitStruct;
	USART_InitTypeDef USART1_InitStruct;
	
	//��GPIOAʱ��
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE );
	
	//��ʼ��PA9\PA10���ù��� ӳ��AF7
	UART1_InitStruct.GPIO_Mode = GPIO_Mode_AF ;//���ù���
	UART1_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init (GPIOA, &UART1_InitStruct);
	
	//ӳ��
	GPIO_PinAFConfig (GPIOA, GPIO_PinSource9, GPIO_AF_USART1);//TX
	GPIO_PinAFConfig (GPIOA, GPIO_PinSource10, GPIO_AF_USART1);//RX
	
	//��USART1ʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE );
	
	//��ʼ��USART1  ��Ҫ�أ������� ����λ  У��λ  ֹͣλ
	USART1_InitStruct.USART_BaudRate = baud;
	USART1_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART1_InitStruct.USART_Parity = USART_Parity_No;//��У��
	USART1_InitStruct.USART_StopBits = USART_StopBits_1 ;//1��ֹͣλ
	USART1_InitStruct.USART_WordLength = USART_WordLength_8b;//8bit
	USART_Init (USART1, &USART1_InitStruct);
	
	//ʹ��USART1
	USART_Cmd (USART1, ENABLE );
}

/********************printf֧�ֺ���****************************/
#pragma import(__use_no_semihosting_swi) //ȡ��������״̬

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
