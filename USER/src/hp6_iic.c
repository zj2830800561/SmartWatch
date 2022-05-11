#include "includes.h"

/***********************************************************
�������ܣ�IIC�ܽų�ʼ��
�����βΣ�None
��������ֵ��None
��ע��
	*PA2--SCL ---�������
	*PA1--SDA----��©���
���ߣ�Yao
���ڣ�2021��01��25��
�汾��V0.0
***********************************************************/
void HP6_IIC_PinInit(void)
{
	GPIO_InitTypeDef GPIOA_InitStruct;
	
	//��ʱ�� AHB1
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE );
	
	//��ʼ��PA2  �������
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//�������
	GPIOA_InitStruct.GPIO_OType = GPIO_OType_PP ;//����
	GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIOA_InitStruct.GPIO_Speed = GPIO_Low_Speed;//2Mhz
	GPIO_Init (GPIOA, &GPIOA_InitStruct);
	
	//PA1 ��©���
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//�������
	GPIOA_InitStruct.GPIO_OType = GPIO_OType_OD ;//��©
	GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init (GPIOA, &GPIOA_InitStruct);
	
	//��ʼ����ߵ�ƽ
	GPIO_SetBits (GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

/*********************************************
�������ܣ�IIC��ʼ����
�����βΣ�None
��������ֵ��None
��ע��*��100KHZΪ��
			*��ʱ���߸ߵ�ƽ�ڼ䣬�����߲����½���  
���ڣ�2020��11��09��
���ߣ�Yao
�汾��V0.0
*********************************************/
void HP6_IIC_Start(void)
{
	HP6_SDA_OUT = 1;
	HP6_SCL = 1;
	Delay_us(IIC_10us);
	HP6_SDA_OUT = 0;//��ʼ���� 
	Delay_us(IIC_10us);
	HP6_SCL = 0;//���һ����������
}

/*********************************************
�������ܣ�IICֹͣ����
�����βΣ�None
��������ֵ��None
��ע��*��100KHZΪ��
			*��ʱ���߸ߵ�ƽ�ڼ䣬�����߲���������  
���ڣ�2020��11��09��
���ߣ�Yao
�汾��V0.0
*********************************************/
void HP6_IIC_Stop(void)
{
	HP6_SDA_OUT = 0;//��ʱ����֮ǰ���������ߣ���ֹ��������֮�����ͻ������ʼ����
	
	HP6_SCL = 1;
	
	Delay_us(IIC_10us);
	HP6_SDA_OUT = 1;//ֹͣ�����������߲���������
	Delay_us(IIC_10us);
}

/*********************************************
�������ܣ���������Ƭ��������Ӧ���ź�
�����βΣ�ack �� �Ƿ�Ӧ��
��������ֵ��None
��ע��*��100KHZΪ��
			*0��Ӧ��  1����Ӧ��
���ڣ�2020��11��09��
���ߣ�Yao
�汾��V0.0
*********************************************/
void HP6_IIC_McuSendAck(u8 ack) // 1  0
{
	HP6_SCL = 0;//���ͷ�-����׼��Ӧ���źŸ���������
	
	if(ack) //����Ϊ��  ack = 1(��Ӧ��)��Ϊ��  ack = 0
		HP6_SDA_OUT = NO_ACK;
	else
		HP6_SDA_OUT = ACK;
	Delay_us(IIC_10us);//����������Ӧ���ź��ȶ�����������
	
	HP6_SCL = 1;//��������ʱ���߲��������� �� ���շ�-�ӻ���ȡӦ���ź�
	Delay_us(IIC_10us);//��ʱ��ӻ���ȡ����Ӧ���ź�
	
	HP6_SCL = 0;//��֤����������
}

/*********************************************
�������ܣ���������Ƭ��������һ��Ӧ���ź�
�����βΣ�None
��������ֵ�� �Ƿ�Ӧ��
��ע��*��100KHZΪ��
			*0��Ӧ��  1����Ӧ��
���ڣ�2020��11��09��
���ߣ�Yao
�汾��V0.0
*********************************************/
u8 HP6_IIC_McuReceiveAck(void)
{
	u8 ret_ack;
	
	HP6_SDA_OUT = 1;//�Ͽ������·
	
	HP6_SCL = 0;//���ͷ�-�ӻ�׼��Ӧ���źŸ���������
	
	Delay_us(IIC_10us);//�ôӻ�����Ӧ���ź��ȶ�����������
	
	HP6_SCL = 1;//��������ʱ���߲��������أ����շ�-������ȡӦ���ź�
	
	//�ж��������ݼĴ��� -IDR ��ֵ
	if( HP6_SDA_IN )  //Ϊ��  == 1 
		ret_ack = NO_ACK;
	else
		ret_ack = ACK;
	
	Delay_us(IIC_10us);//��ʱ��ӻ���ȡ����Ӧ���ź�
	
	HP6_SCL = 0;
	
	return ret_ack;
}

/*********************************************
�������ܣ���������Ƭ��������һ���ֽں���
�����βΣ�������һ���ֽ�����
��������ֵ�� �ӻ��Ƿ�Ӧ��
��ע��*��100KHZΪ��
			*0��Ӧ��  1����Ӧ��
			������Ϊ���ͷ����ӻ���Ϊ���շ�
���ڣ�2020��11��09��
���ߣ�Yao
�汾��V0.0
*********************************************/
u8 HP6_IIC_McuSendByte(u8 send_data)
{
	u8 ret_ack;
	u8 i;
	
	for(i = 0; i < 8; i++)
	{
		HP6_SCL = 0;//����׼������
		
		if(send_data & (0x80 >> i)) // 1000 0000
			HP6_SDA_OUT = 1;
		else	
			HP6_SDA_OUT = 0;
		
		Delay_us(IIC_10us);//�������ȶ�����������
		
		HP6_SCL = 1;//ʱ���߲��������أ����շ�-�ӻ���ȡ����
		Delay_us(IIC_10us);//��ʱ��ӻ��ɼ�����
	}
	
	HP6_SCL = 0;
	
	ret_ack = HP6_IIC_McuReceiveAck( );//��ȡ�ӻ���Ӧ���ź�
	
	return ret_ack;
}

/*********************************************
�������ܣ���������Ƭ��������һ���ֽں���
�����βΣ����������Ƿ�Ӧ��
��������ֵ�� ���յ���һ���ֽ�����
��ע��*��100KHZΪ��
			*0��Ӧ��  1����Ӧ��
			������Ϊ���շ����ӻ���Ϊ���ͷ�
			��������Ӧ���ź�
���ڣ�2020��11��09��
���ߣ�Yao
�汾��V0.0
*********************************************/
u8 HP6_IIC_McuReceiveByte(u8 ack)
{
	u8 recv_data;
	u8 i;
	
	HP6_SDA_OUT = 1;//�Ͽ������·
	
	for(i = 0; i < 8; i++)
	{
		HP6_SCL = 0;//�½��شӻ�׼������
		Delay_us(IIC_10us);//�������ȶ�����������
		
		HP6_SCL = 1;//ʱ���߲��������أ����շ�-������ȡ����
		
		recv_data <<= 1;//�ճ����λ����һλ����
		recv_data |= HP6_SDA_IN; //��ȡ�������ݼĴ�����9λ��ֵ���浽���������λ
		
		Delay_us(IIC_10us);//��ʱ�������ɼ�����
	}
	
	HP6_SCL = 0;
	
	HP6_IIC_McuSendAck(ack);//���������Ƿ�Ӧ��
	
	return recv_data;//���ؽ��յ���һ���ֽ�����
}

/*********************************************
�������ܣ�IIC����д�ֽں���
�����βΣ�addr:�ӻ���ַ
					t_buf:�����͵�����
					size:����size�ֽ�
��������ֵ�� ���յ���size���ֽ�����
��ע��*��100KHZΪ��
			*0�����ͳɹ�  1������ʧ��
���ڣ�2020��12��01��
���ߣ�Yao
�汾��V0.0
*********************************************/
u8 HP6_IIC_McuSendData(u8 addr, u8 *t_buf, u8 size)
{
	u8 i, ack;
	
	HP6_IIC_Start( );//��ʼ����
	
	ack = HP6_IIC_McuSendByte(addr << 1);//������ַ+д
	if(ack)
			return 1;
	
	for(i = 0; i < size; i++)
	{
		ack = HP6_IIC_McuSendByte(*t_buf++);
		if(ack)
			return 2;
	}
	
	HP6_IIC_Stop( );//ֹͣ����

	return 0;
}

/*********************************************
�������ܣ�IIC���������ֽں���
�����βΣ�addr:�ӻ���ַ
					t_buf:�����͵�����
					size:����size�ֽ�
��������ֵ�� ���յ���size���ֽ�����
��ע��*��100KHZΪ��
			*0�����ճɹ�  ����������ʧ��
			������Ϊ���շ����ӻ���Ϊ���ͷ�
			��������Ӧ���ź�
���ڣ�2020��12��01��
���ߣ�Yao
�汾��V0.0
*********************************************/
u8 HP6_IIC_McuReadData(u8 addr, u8 *r_buf, u8 size)
{
	u8 i, ack;
	
	HP6_IIC_Start( );//��ʼ����
	
	ack = HP6_IIC_McuSendByte((addr << 1)|1);//������ַ+��
	if(ack)
			return 3;
	
	for(i = 0; i < size-1; i++)
	{
		*r_buf++ = HP6_IIC_McuReceiveByte(ACK);
	}
	
	*r_buf = HP6_IIC_McuReceiveByte(NO_ACK);
	
	HP6_IIC_Stop( );//ֹͣ����
	
	return 0;
}
