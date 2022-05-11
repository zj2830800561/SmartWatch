#include "main.h"

int main(void)
{
	u8 i, flag= 1;
	uint8_t ret,PowerOn_Flag=0;
	u8 buffer[24];
	Led_Init( );//LED初始化
	KEY_Init( );//按键初始化
	UART1_Init(115200);//115200
	OLED_Init( );//OLED初始化
	SHT20_Init( );//SHT20初始化
	HP6_Init( );//HP6初始化

	printf("hello zj\r\n");
	printf("功能初始化中- - - \r\n");
	printf("请稍后- - - \r\n");
	HP_6_OpenRate( );//打开心率测量
	Delay_ms(5000);//等待心率测量开启
	HP_6_GetResultData(buffer);
	if(buffer[6] == 1)
		printf("心率测量开启成功\r\n");
	else
		printf("心率测量开启失败\r\n");
	//HP_6_OpenBp();//打开血压测量
//	Delay_ms(3000);//等待血压测量开启
//	HP_6_GetResultData(buffer);
//	if(buffer[6] == 1)
//		printf("血压测量开启成功\r\n");
//	else
//		printf("血压测量开启失败\r\n");
	
	while(1)
	{
		ret=Key_Scan();
		switch(ret)
		{
			case Key_OK:
				printf("\r\n功能选择\r\n\t1、按键向左选择心率测量\r\n\t2、按键向右选择血压测量");
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
						printf("\r\n\r\n选择心率测量，3秒后开始测量\r\n");
						Delay_ms(4000);
						while(1){
							HP_6_GetRateResult();
							//HP_6_GetResultData(buffer);
							if(rx_buf[7] == 0)
							{
								for(i = 0; i < 24; i++)
									printf("%d ", rx_buf[i]);
								printf("\r\n");
								printf("测量中...\r\n");
							}
							else
							{
								printf("心率测量结果为：%d\r\n", rx_buf[7]);
								//HP_6_CloseRate();
								printf("\r\n功能选择\r\n\t1、按键向左选择心率测量\r\n\t2、按键向右选择血压测量");
								break;
							}
							Delay_ms(2000);
						}
						break;
					case Key_RIGHT:
						printf("\r\n\r\n选择血压测量，3秒后开始测量\r\n");
						Delay_ms(4000);
						while(1){
							HP_6_GetBpResult();
							HP_6_GetResultData(buffer);
							if(buffer[7] == 1)
							{
								printf("高压：%d\r\n",buffer[10]);
								printf("低压：%d\r\n",buffer[11]);
								//HP_6_CloseBp();
								printf("\r\n功能选择\r\n\t1、按键向左选择心率测量\r\n\t2、按键向右选择血压测量");
								break;
							}
							else if(buffer[7] == 0)
							{
								printf("测量中...\r\n");
							}
							else
							{
								printf("测量失败\r\n");
								//HP_6_CloseBp();
								printf("\r\n功能选择\r\n\t1、按键向左选择心率测量\r\n\t2、按键向右选择血压测量");
								break;
							}
							Delay_ms(2000);
						}
						break;
				}
			}
		}
		else{
			printf("按下OK键开始选择\r\n");
			Delay_ms(3000);
		}
		
	}
}
