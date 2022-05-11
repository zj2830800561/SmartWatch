#include "includes.h"

/*******************************************
�������ܣ�SPI��������ʼ��
�����βΣ�None
��������ֵ��None
��ע��OLED_SCL --- PB3  ���ù���  AF5
			OLED_MOSI -- PB5	���ù���  AF5
			OLED_CS		-- PB7  �������	
���ڣ�2021��03��05��
���ߣ�Yao
�汾��V0.0
*******************************************/
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIOB_InitStruct;
	SPI_InitTypeDef SPI1_InitStruct;
	
	//��GPIOBʱ��
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB, ENABLE );
	
	//��ʼ��PB7 �������
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//�������
	GPIOB_InitStruct.GPIO_OType = GPIO_OType_PP ;//����
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIOB_InitStruct.GPIO_Speed = GPIO_Medium_Speed;//����  10Mhz
	GPIO_Init (GPIOB, &GPIOB_InitStruct);
	
	//��ʼ��PB3/PB5  ���ù���  AF5
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_AF ;//���ù���
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_Init (GPIOB, &GPIOB_InitStruct);
	
	//ӳ��
	GPIO_PinAFConfig (GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig (GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	//��SPI1������ʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SPI1, ENABLE );
	
	//��ʼ��SPI1������  --  MODE3  ��λ��ǰ
	SPI1_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//fPCLK/2
	SPI1_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;//���زɼ�����
	SPI1_InitStruct.SPI_CPOL = SPI_CPOL_High;//���е�ƽΪ�ߵ�ƽ
	SPI1_InitStruct.SPI_DataSize = SPI_DataSize_8b;//8bit
	SPI1_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//����������  ȫ˫��  -- 4��SPI
	SPI1_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//��λ��ǰ
	SPI1_InitStruct.SPI_Mode = SPI_Mode_Master;//MCU��Ϊ����
	SPI1_InitStruct.SPI_NSS = SPI_NSS_Soft;//ʹ���������������
	SPI_Init (SPI1, &SPI1_InitStruct);
	
	//ʹ��SPI1
	SPI_Cmd (SPI1, ENABLE );
}


/*******************************************
�������ܣ�SPI���������� 1 ���ֽں���
�����βΣ�u8 sData: �����͵�һ���ֽ�����
��������ֵ��None
��ע���������ݣ��ȴ����ͻ�����Ϊ��	
			�������ݣ��ȴ����ջ���ȥ�ǿ�
���ڣ�2021��03��05��
���ߣ�Yao
�汾��V0.0
*******************************************/
void SPI1_SendByte(u8 sData)
{
	//�ȴ����ͻ�����Ϊ��
	while(SPI_I2S_GetFlagStatus (SPI1, SPI_I2S_FLAG_TXE) == RESET);
	
	SPI_I2S_SendData (SPI1, sData);//��������
	
	//�ȴ����ջ������ǿ�
	while(SPI_I2S_GetFlagStatus (SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
	//sData = SPI_I2S_ReceiveData (SPI1);//��������
}


