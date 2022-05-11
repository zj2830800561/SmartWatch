#ifndef _SHT20_H_
#define _SHT20_H_

#include "stm32f4xx.h"

#define ADDR_W 	0x80
#define ADDR_R	0x81
#define SOFT_RESET	0xfe
#define RESET_TIME	15

#define TEMP_MEASURE 0xf3
#define HUMI_MEASURE 0xf5


u8 SHT20_Init(void);
float SHT20_MeasureCmd(u8 cmd);

#endif


