#include "main.h"

int main(void)
{
	u8 i, flag= 1;
	uint8_t ret,PowerOn_Flag=0;
	u8 buffer[24];
	Led_Init( );//LED��ʼ��
	KEY_Init( );//������ʼ��
	UART1_Init(115200);//115200
	OLED_Init( );//OLED��ʼ��
	SHT20_Init( );//SHT20��ʼ��
	HP6_Init( );//HP6��ʼ��

	printf("hello zj\r\n");
	printf("���ܳ�ʼ����- - - \r\n");
	printf("���Ժ�- - - \r\n");
	HP_6_OpenRate( );//�����ʲ���
	Delay_ms(5000);//�ȴ����ʲ�������
	HP_6_GetResultData(buffer);
	if(buffer[6] == 1)
		printf("���ʲ��������ɹ�\r\n");
	else
		printf("���ʲ�������ʧ��\r\n");
	//HP_6_OpenBp();//��Ѫѹ����
//	Delay_ms(3000);//�ȴ�Ѫѹ��������
//	HP_6_GetResultData(buffer);
//	if(buffer[6] == 1)
//		printf("Ѫѹ���������ɹ�\r\n");
//	else
//		printf("Ѫѹ��������ʧ��\r\n");
	
	while(1)
	{
		ret=Key_Scan();
		switch(ret)
		{
			case Key_OK:
				printf("\r\n����ѡ��\r\n\t1����������ѡ�����ʲ���\r\n\t2����������ѡ��Ѫѹ����");
				PowerOn_Flag = 1;
				break;
		}
		if(PowerOn_Flag)
		{
			while(1){
				ret=Key_Scan();
				switch(ret)
				{
					case KeY_LEFT:
						printf("\r\n\r\nѡ�����ʲ�����3���ʼ����\r\n");
						Delay_ms(4000);
						while(1){
							HP_6_GetRateResult();
							//HP_6_GetResultData(buffer);
							if(rx_buf[7] == 0)
							{
								for(i = 0; i < 24; i++)
									printf("%d ", rx_buf[i]);
								printf("\r\n");
								printf("������...\r\n");
							}
							else
							{
								printf("���ʲ������Ϊ��%d\r\n", rx_buf[7]);
								//HP_6_CloseRate();
								printf("\r\n����ѡ��\r\n\t1����������ѡ�����ʲ���\r\n\t2����������ѡ��Ѫѹ����");
								break;
							}
							Delay_ms(2000);
						}
						break;
					case Key_RIGHT:
						printf("\r\n\r\nѡ��Ѫѹ������3���ʼ����\r\n");
						Delay_ms(4000);
						while(1){
							HP_6_GetBpResult();
							HP_6_GetResultData(buffer);
							if(buffer[7] == 1)
							{
								printf("��ѹ��%d\r\n",buffer[10]);
								printf("��ѹ��%d\r\n",buffer[11]);
								//HP_6_CloseBp();
								printf("\r\n����ѡ��\r\n\t1����������ѡ�����ʲ���\r\n\t2����������ѡ��Ѫѹ����");
								break;
							}
							else if(buffer[7] == 0)
							{
								printf("������...\r\n");
							}
							else
							{
								printf("����ʧ��\r\n");
								//HP_6_CloseBp();
								printf("\r\n����ѡ��\r\n\t1����������ѡ�����ʲ���\r\n\t2����������ѡ��Ѫѹ����");
								break;
							}
							Delay_ms(2000);
						}
						break;
				}
			}
		}
		else{
			printf("����OK����ʼѡ��\r\n");
			Delay_ms(3000);
		}
		
	}
}
