#include "includes.h"


/*******************************************
�������ܣ�OLED�ܽų�ʼ��
�����βΣ�None
��������ֵ��None
��ע��OLED_SCL --- PB3  ���ù���  AF5
			OLED_MOSI -- PB5	���ù���  AF5
			OLED_CS		-- PB7  �������	
			OLED_DC 	-- PA15 �������
			OLED_RES	-- PB13	�������
���ڣ�2021��03��05��
���ߣ�Yao
�汾��V0.0
*******************************************/
void OLED_PinInit(void)
{
	GPIO_InitTypeDef GPIOB_InitStruct;
	
	/******************SPI�ܽų�ʼ��*************************/
	SPI1_Init( );
	
	/******************OLED�ܽų�ʼ��*************************/
	//��GPIOB/GPIOAʱ��
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE );
	
	//��ʼ��PB13 �������
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//�������
	GPIOB_InitStruct.GPIO_OType = GPIO_OType_PP ;//����
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIOB_InitStruct.GPIO_Speed = GPIO_Medium_Speed;//����  10Mhz
	GPIO_Init (GPIOB, &GPIOB_InitStruct);
	
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init (GPIOA, &GPIOB_InitStruct);//PA15
}

/*******************************************
�������ܣ�OLEDдһ���ֽں���
�����βΣ�
				*u8 sData:�����͵�һ���ֽ�����
				*u8 cmd:��������ѡ��
��������ֵ��None
��ע��DC = 0:��������
			DC = 1:��������
���ڣ�2021��03��05��
���ߣ�Yao
�汾��V0.0
*******************************************/
void OLED_WriteByte(u8 sData, u8 cmd)
{	
	OLED_CS = 0;//����Ƭѡ
	
	OLED_DC = cmd;
	
	SPI1_SendByte(sData);//����һ���ֽ�����/����
	
	OLED_CS = 1;//����Ƭѡ
}

/*******************************************
�������ܣ�OLED������ʾλ�ú���
�����βΣ�
				*u8 page:ҳ��ַ
				*u8 col:�е�ַ
��������ֵ��None
��ע��ҳ��ַ��0xBx
			�е͵�ַ��0x0x  �иߵ�ַ��0x1x
			BUG:��OLED��ǰ�����޷�������ʾ
���ڣ�2021��03��05��
���ߣ�Yao
�汾��V0.0
*******************************************/
void OLED_SetPos(u8 page, u8 col)
{
	col += 2;//�е�ַƫ������  ��1��2���޷�������ʾ
	
	OLED_WriteByte(0xB0 | page, OLED_CMD);//����ҳ��ַ
	OLED_WriteByte(0x00 | (col & 0x0f), OLED_CMD);//�����е�ַ����λ
	OLED_WriteByte(0x10 | ((col & 0xf0)>>4), OLED_CMD);//�����е�ַ����λ
}

/*******************************************
�������ܣ�OLED��������
�����βΣ�None
��������ֵ��None
��ע��130*64
			BUG:��OLED��ǰ�����޷�������ʾ
���ڣ�2021��03��05��
���ߣ�Yao
�汾��V0.0
*******************************************/
void OLED_Clear(void)
{
	u8 i, j;
	
	for(i = 0; i < 8; i++)//������ʾҳ��
	{
		//������ʾλ��
		OLED_SetPos(i, 0);
		
		//������ʾ����
		for(j = 0; j < 130; j++)//������ʾ����
		{
			OLED_WriteByte(0x00, OLED_DATA);//������ʾ����
		}
	}
}


/*******************************************
�������ܣ�OLED��ʼ�����к���
�����βΣ�None
��������ֵ��None
��ע��None
���ڣ�2021��03��05��
���ߣ�Yao
�汾��V0.0
*******************************************/
void OLED_Init(void)
{ 
	OLED_PinInit();	//�ܽų�ʼ��
	
	//��λOLED
	OLED_RES = 0;
	Delay_ms(100);
	OLED_RES = 1;
	
	OLED_WriteByte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WriteByte(0x02,OLED_CMD);//---SET low column address
	OLED_WriteByte(0x10,OLED_CMD);//---SET high column address
	OLED_WriteByte(0x40,OLED_CMD);//--SET start line address  SET Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteByte(0x81,OLED_CMD);//--SET contrast control register
	OLED_WriteByte(0xCF,OLED_CMD); // SET SEG Output Current Brightness
	OLED_WriteByte(0xA1,OLED_CMD);//--SET SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WriteByte(0xC8,OLED_CMD);//SET COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WriteByte(0xA6,OLED_CMD);//--SET normal display
	OLED_WriteByte(0xA8,OLED_CMD);//--SET multiplex ratio(1 to 64)
	OLED_WriteByte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WriteByte(0xD3,OLED_CMD);//-SET display offSET	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteByte(0x00,OLED_CMD);//-not offSET
	OLED_WriteByte(0xd5,OLED_CMD);//--SET display clock divide ratio/oscillator frequency
	OLED_WriteByte(0x80,OLED_CMD);//--SET divide ratio, SET Clock as 100 Frames/Sec
	OLED_WriteByte(0xD9,OLED_CMD);//--SET pre-charge period
	OLED_WriteByte(0xF1,OLED_CMD);//SET Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WriteByte(0xDA,OLED_CMD);//--SET com pins hardware configuration
	OLED_WriteByte(0x12,OLED_CMD);
	OLED_WriteByte(0xDB,OLED_CMD);//--SET vcomh
	OLED_WriteByte(0x40,OLED_CMD);//SET VCOM Deselect Level
	OLED_WriteByte(0x20,OLED_CMD);//-SET Page Addressing Mode (0x00/0x01/0x02)
	OLED_WriteByte(0x02,OLED_CMD);//
	OLED_WriteByte(0x8D,OLED_CMD);//--SET Charge Pump enable/disable
	OLED_WriteByte(0x14,OLED_CMD);//--SET(0x10) disable
	OLED_WriteByte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WriteByte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WriteByte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WriteByte(0xAF,OLED_CMD); /*display ON*/ 
		
	OLED_Clear();
}  


/*******************************************
�������ܣ�OLED��ʾ���⺯��
�����βΣ���ʾλ�ã���ʾ���ݣ������С����Ӧ��С���ֿ�
��������ֵ��None
��ע��16*8  16   32*16   32
���ڣ�2021��03��05��
���ߣ�Yao
�汾��V0.0
*******************************************/
void OLED_DisAscii(u8 page, u8 col, u8 ch, u8 size, u8 *font)
{
	u8 i, j;
	u8 pos;
	
	pos = ch - ' ';	
	
	for(i = 0; i < size/8; i++)//������ʾҳ��
	{
		//������ʾ�ĳ�ʼλ��
		OLED_SetPos(page+i, col);
		
		for(j = 0; j < size/2; j++)//������ʾ����  for(j = 0; j < 8; j++)
		{
			OLED_WriteByte(font[pos*(size*size/16) + j + i*(size/2)], OLED_DATA);//������ʾ������
			//size*size/16  ---- һ��size�ַ���Ӧ����ģ���ݸ���
		}
	}
}


/*******************************************
�������ܣ�OLED��ʾ���⺺���ַ�����
�����βΣ���ʾλ�ã���ʾ���ݣ������С��λ���ֿ⣬��Ӧ��С���ֿ�
��������ֵ��None
��ע��16*16  16   32*32   32
���ڣ�2021��03��05��
���ߣ�Yao
�汾��V0.0
*******************************************/
void OLED_DisChinese(u8 page, u8 col, u8 *hz, u8 size, u8 *font_m)
{
	u8 i, j;
	u8 pos = 0;
	
	for(i = 0; i < (strlen((const char *)chinese_font)/2); i++)//���������ֿ�Ѱ�Һ���
	{
		if((hz[0]==chinese_font[i*2]) && (hz[1]==chinese_font[i*2+1]))
		{
			pos = i;//�ҵ����ֵ�λ��
			break;
		}
	}
	
	if(pos != i)//û�ҵ�
	{
		printf("display error\r\n");
		return ;
	}		
	
	for(i = 0; i < size/8; i++)//������ʾҳ��
	{
		//������ʾ�ĳ�ʼλ��
		OLED_SetPos(page+i, col);
		
		for(j = 0; j < size; j++)//������ʾ����  for(j = 0; j < 8; j++)
		{
			OLED_WriteByte(font_m[pos*(size*size/8) + j + i*size], OLED_DATA);//������ʾ������
			//size*size/8 ---- һ��size�ַ���Ӧ����ģ���ݸ���
		}
	}
}


/*******************************************
�������ܣ�OLED��ʾ�ַ�������
�����βΣ���ʾλ�ã���ʾ���ݣ������С����Ӧ��С���ֿ�
��������ֵ��None
��ע��16*8  16   32*16   32
���ڣ�2021��03��05��
���ߣ�Yao
�汾��V0.0
*******************************************/
void OLED_DisString(u8 page, u8 col, u8 *str, u8 size, u8 *font_e, u8 *font_c)
{
	while(*str != '\0')
	{
		//�ж���ʲô�ַ�
		if(*str < 128) //Ӣ���ַ�
		{
			OLED_DisAscii(page, col, *str, size, font_e);//��ʾӢ���ַ�
			
			str++;
			col += size/2;
		}else if((*str > 0xa0) && (*(str+1) > 0xa0)) //�����ַ�
		{
			OLED_DisChinese(page, col, str, size, font_c);//��ʾ�����ַ�
			
			str += 2;
			col += size;
		}
		
		if(*str < 128)
		{
			if(col > (130 - size/2))
			{
				page += size/8;//��ҳ
				col = 0;//�е�ַ����
			}
		}else if((*str > 0xa0) && (*(str+1) > 0xa0))
		{
			if(col > (130 - size))
			{
				page += size/8;//��ҳ
				col = 0;//�е�ַ����
			}
		}
	}
}


