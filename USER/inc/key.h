#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f4xx.h"
#include "io_bit.h"

#define KEY_OK PAin(0)

#define Key_NO 		0
#define Key_OK 		1
#define KeY_LEFT 	2
#define Key_RIGHT	3
#define Key_UP		4
#define Key_DOWN	5

void KEY_Init(void);
u8 Key_Scan(void);

#endif

