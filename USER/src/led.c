#include "includes.h"

/*******************************************
�������ܣ�led��ʼ��
�����βΣ�None
��������ֵ��None
��ע��PA7-LED
���ڣ�2021��03��03��
���ߣ�Yao
�汾��V0.0
*******************************************/
void Led_Init(void)
{
	GPIO_InitTypeDef GPIOA_InitStruct;
	
	//��ʱ�� AHB1
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE );
	
	//��ʼ��PA7
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//�������
	GPIOA_InitStruct.GPIO_OType = GPIO_OType_PP ;//����
	GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIOA_InitStruct.GPIO_Speed = GPIO_Low_Speed;//2Mhz
	GPIO_Init (GPIOA, &GPIOA_InitStruct);
	
	//��ʼ����ߵ�ƽ
	GPIO_SetBits (GPIOA, GPIO_Pin_7);
}
