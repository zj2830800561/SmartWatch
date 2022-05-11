#ifndef _LED_H_
#define _LED_H_

#include "stm32f4xx.h"
#include "io_bit.h"


#define LED PAout(7)

void Led_Init(void);


#endif

