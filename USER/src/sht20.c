#include "includes.h"


/*******************************************
�������ܣ�SHT20��λ����
�����βΣ�None
��������ֵ��0:����  ������ʧ��
��ע��None
���ڣ�2021��03��08��
���ߣ�Yao
�汾��V0.0
*******************************************/
u8 SHT20_SoftReset(void)
{
	u8 ack;
	
	//������ʼ����
	IIC_Start( );
	
	//����������ַ+д
	ack = IIC_McuSendByte(ADDR_W);
	if(ack)
	{
		return 1;//��Ӧ��
	}
	
	//��������λָ��
	ack = IIC_McuSendByte(SOFT_RESET);
	if(ack)
	{
		return 2;//��Ӧ��
	}
	
	//����ֹͣ����
	IIC_Stop( );
	
	return 0;
}


/*******************************************
�������ܣ�SHT20��ʼ������
�����βΣ�None
��������ֵ��0:����  ������ʧ��
��ע���ܽ� + ��λ
���ڣ�2021��03��08��
���ߣ�Yao
�汾��V0.0
*******************************************/
u8 SHT20_Init(void)
{
	u8 ret;
	
	IIC_PinInit( );//SHT20 IIC�ܽų�ʼ��
	
	//��λ
	ret = SHT20_SoftReset( );
	if(ret)
	{
		return 3;//��λʧ��
	}
	
	//��ʱ15ms
	Delay_ms(RESET_TIME);
	
	return 0;
}

/*******************************************
�������ܣ�SHT20��������
�����βΣ������¶�/ʪ��
��������ֵ�����ز�������:����  ������ʧ��
��ע��None
���ڣ�2021��03��08��
���ߣ�Yao
�汾��V0.0
*******************************************/
float SHT20_MeasureCmd(u8 cmd)
{
	u8 ack;
	u16 rData;
	
	IIC_Start( );//������ʼ����
	
	ack = IIC_McuSendByte(ADDR_W);//����������ַ+д
	if(ack)
	{
		return 4;//��Ӧ��
	}
	
	ack = IIC_McuSendByte(cmd);//���Ͳ�������
	if(ack)
	{
		return 5;//��Ӧ��
	}
	
	do
	{
		IIC_Start( );//������ʼ����
		ack = IIC_McuSendByte(ADDR_R);//����������ַ+��
		Delay_ms(10);
	}while(ack);
	
	//�������
	rData = IIC_McuReceiveByte(ACK);//���ֽ�
	rData <<= 8;
	rData |= IIC_McuReceiveByte(NO_ACK);//���ֽ�
	
	IIC_Stop( );//����ֹͣ����
	
	rData &= ~(0x3 << 0);//��������λ״̬�����л���
	
	if(cmd == TEMP_MEASURE)
	{
		return (175.72*rData/65536 - 46.85);
	}else if(cmd == HUMI_MEASURE)
	{
		return (125.0*rData/65536.0 - 6.0);
	}
	
	return 6;//����ָ������
}

