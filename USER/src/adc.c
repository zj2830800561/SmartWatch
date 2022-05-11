#include "includes.h"

/*******************************************
函数功能：KEY_ADIN初始化
函数形参：None
函数返回值：None
备注：PA3-KEY_ADIN ---- AHB1
			ADC1 ---- APB2
日期：2021年03月04日
作者：Yao
版本：V0.0
*******************************************/
void Adc1_Init(void)
{
	GPIO_InitTypeDef GPIOA_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	
	//1. 打开 GPIOA 时钟
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE );
	
	//2. 初始化 PA3 – 模拟功能
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_AN ;//模拟功能
	GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init (GPIOA, &GPIOA_InitStruct);
	
	//3. 打开 ADC1 时钟 --- APB2
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1, ENABLE );
	
	//4. 配置 ADC1 时钟分频 --- ADCCLK CCR
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//禁用DMA
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//单ADC模式
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div6;//6分频 6Mhz
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//采样延时
	ADC_CommonInit (&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//连续转换模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
	ADC_InitStruct.ADC_NbrOfConversion = 1;//通道总数1
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//12bit分辨率
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//禁用扫描模式
	ADC_Init (ADC1, &ADC_InitStruct);
	
	//构造规则组
	ADC_RegularChannelConfig (ADC1, ADC_Channel_3, 1, ADC_SampleTime_480Cycles);//通道3  第一次转换  480个时钟周期
	
	//8. 配置转换结束中断
	//a) 设置中断优先级
	NVIC_SetPriority(ADC_IRQn, NVIC_EncodePriority(7-2, 0, 1));
	
	//b) 使能 NVIC 响应
	NVIC_EnableIRQ(ADC_IRQn);
	
	//c) 使能转换结束中断
	ADC_ITConfig (ADC1, ADC_IT_EOC, ENABLE);
	
	//9. 使能 ADC1
	ADC_Cmd (ADC1, ENABLE);
	
	//10. 触发规则组转换
	ADC_SoftwareStartConv (ADC1);
}	


u16 keyAD[10];

//11. 编写中断服务函数
void ADC_IRQHandler(void)
{
	static u8 cnt = 0;
	u8 i = 0, j = 0;
	u16 temp = 0;
	
	if(ADC1->SR & (0x1 << 1))//规则组转换结束中断
	{
		//清除EOC标志
		ADC1->SR &= ~(0x1 << 1);
		
		//采集转换完成的值
		keyAD[cnt++] = ADC1->DR;
		
		//处理采集值
		if(cnt == 10)//采集十次
		{
			cnt = 0;//清零计数值
			
			//排序
			for(i = 0; i < 9; i++)//控制比较趟数
			{
				for(j = 0; j < 9-i; j++)//控制每趟比较的次数
				{
					if(keyAD[j] > keyAD[j+1])//从小到大
					{
						temp = keyAD[j];
						keyAD[j] = keyAD[j+1];
						keyAD[j+1] = temp;
					}
				}
			}
			
			keyAD[0] = 0;//清零最小值  用此位保存 和值
			for(i = 1; i < 9; i++) 
			{
				keyAD[0] += keyAD[i];//去掉最大值和最小值求和
			}
			keyAD[0] = keyAD[0] / 8;//取平均值
			
			//printf("AD: %d\r\n", keyAD[0]);
		}
	}
}

//a) 采集转换完成的数据 10 次
//b) 处理数据： 去掉最大值去掉最小值（冒泡排序）， 取平均值



