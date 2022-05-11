#ifndef _OLED_H_
#define _OLED_H_

#include "stm32f4xx.h"
#include "io_bit.h"

#define OLED_CS 	PBout(7)
#define OLED_DC 	PAout(15)
#define OLED_RES 	PBout(13)
 
#define OLED_CMD	0
#define OLED_DATA	1


void OLED_Init(void);
void OLED_DisAscii(u8 page, u8 col, u8 ch, u8 size, u8 *font);
void OLED_DisChinese(u8 page, u8 col, u8 *hz, u8 size, u8 *font_m);
void OLED_DisString(u8 page, u8 col, u8 *str, u8 size, u8 *font_e, u8 *font_c);

#endif


