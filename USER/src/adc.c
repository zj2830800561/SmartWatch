#include "includes.h"

/*******************************************
�������ܣ�KEY_ADIN��ʼ��
�����βΣ�None
��������ֵ��None
��ע��PA3-KEY_ADIN ---- AHB1
			ADC1 ---- APB2
���ڣ�2021��03��04��
���ߣ�Yao
�汾��V0.0
*******************************************/
void Adc1_Init(void)
{
	GPIO_InitTypeDef GPIOA_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	
	//1. �� GPIOA ʱ��
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE );
	
	//2. ��ʼ�� PA3 �C ģ�⹦��
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_AN ;//ģ�⹦��
	GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init (GPIOA, &GPIOA_InitStruct);
	
	//3. �� ADC1 ʱ�� --- APB2
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1, ENABLE );
	
	//4. ���� ADC1 ʱ�ӷ�Ƶ --- ADCCLK CCR
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//����DMA
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//��ADCģʽ
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div6;//6��Ƶ 6Mhz
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//������ʱ
	ADC_CommonInit (&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//����ת��ģʽ
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//�����Ҷ���
	ADC_InitStruct.ADC_NbrOfConversion = 1;//ͨ������1
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//12bit�ֱ���
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//����ɨ��ģʽ
	ADC_Init (ADC1, &ADC_InitStruct);
	
	//���������
	ADC_RegularChannelConfig (ADC1, ADC_Channel_3, 1, ADC_SampleTime_480Cycles);//ͨ��3  ��һ��ת��  480��ʱ������
	
	//8. ����ת�������ж�
	//a) �����ж����ȼ�
	NVIC_SetPriority(ADC_IRQn, NVIC_EncodePriority(7-2, 0, 1));
	
	//b) ʹ�� NVIC ��Ӧ
	NVIC_EnableIRQ(ADC_IRQn);
	
	//c) ʹ��ת�������ж�
	ADC_ITConfig (ADC1, ADC_IT_EOC, ENABLE);
	
	//9. ʹ�� ADC1
	ADC_Cmd (ADC1, ENABLE);
	
	//10. ����������ת��
	ADC_SoftwareStartConv (ADC1);
}	


u16 keyAD[10];

//11. ��д�жϷ�����
void ADC_IRQHandler(void)
{
	static u8 cnt = 0;
	u8 i = 0, j = 0;
	u16 temp = 0;
	
	if(ADC1->SR & (0x1 << 1))//������ת�������ж�
	{
		//���EOC��־
		ADC1->SR &= ~(0x1 << 1);
		
		//�ɼ�ת����ɵ�ֵ
		keyAD[cnt++] = ADC1->DR;
		
		//����ɼ�ֵ
		if(cnt == 10)//�ɼ�ʮ��
		{
			cnt = 0;//�������ֵ
			
			//����
			for(i = 0; i < 9; i++)//���ƱȽ�����
			{
				for(j = 0; j < 9-i; j++)//����ÿ�˱ȽϵĴ���
				{
					if(keyAD[j] > keyAD[j+1])//��С����
					{
						temp = keyAD[j];
						keyAD[j] = keyAD[j+1];
						keyAD[j+1] = temp;
					}
				}
			}
			
			keyAD[0] = 0;//������Сֵ  �ô�λ���� ��ֵ
			for(i = 1; i < 9; i++) 
			{
				keyAD[0] += keyAD[i];//ȥ�����ֵ����Сֵ���
			}
			keyAD[0] = keyAD[0] / 8;//ȡƽ��ֵ
			
			//printf("AD: %d\r\n", keyAD[0]);
		}
	}
}

//a) �ɼ�ת����ɵ����� 10 ��
//b) �������ݣ� ȥ�����ֵȥ����Сֵ��ð�����򣩣� ȡƽ��ֵ



