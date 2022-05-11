#include "includes.h"

/*******************************************
�������ܣ�KEY��ʼ��
�����βΣ�None
��������ֵ��None
��ע��PA0-KEY_OK  AHB1
���ڣ�2021��03��03��
���ߣ�Yao
�汾��V0.0
*******************************************/
void KEY_Init(void)
{
	/*********************KEY_OK��ʼ��**************************/
	GPIO_InitTypeDef GPIOA_InitStruct;
	
	//��ʱ�� AHB1
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE );
	
	//��ʼ��PA0
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_IN;//���빦��
	GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIOA_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
	GPIO_Init (GPIOA, &GPIOA_InitStruct);
	
	/*********************KEY_ADIN��ʼ��**************************/
	Adc1_Init( );
}


/*******************************************
�������ܣ�����ɨ�躯��
�����βΣ�None
��������ֵ�����£����ؼ���  û�У�����0
��ע��PA0-KEY_OK  AHB1   �����Ǹߵ�ƽ
���ڣ�2021��03��03��
���ߣ�Yao
�汾��V0.0
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
		//��ʱ���� 10ms
		
		if(KEY_OK == 1)//����
		{
			flag = 1;			
			return Key_OK;
		}
	}else if((KEY_OK == 0) && (flag == 1))//����
	{
		flag = 0;
	}
	
	return Key_NO;
}

