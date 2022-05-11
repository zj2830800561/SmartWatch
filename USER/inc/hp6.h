#ifndef _HP6_H_
#define _HP6_H_

#include "stm32f4xx.h"

#define PROTOCLO_HEADER			0xc8d7b6a5
#define PROTOCLO_CMD_INDEX		4
#define PROTOCLO_DATA_INDEX		5
#define PROTOCLO_BUF_MAX_LEN	24

#define HP_6_FAILURE			0x00
#define HP_6_SUCCESS			0x01

#define HP_6_IIC_ADDRESS 		  0x66
#define HP_6_IIC_ADDRESS_W 		0xcc
#define HP_6_IIC_ADDRESS_R 		0xcd

#define HP6_EN PCout(13)

extern uint8_t rx_buf[PROTOCLO_BUF_MAX_LEN];	

void HP6_Init(void); 
uint16_t HP_6_GetResultData(uint8_t *data);

uint8_t HP_6_GetRateResult(void);
uint8_t HP_6_OpenRate(void);
uint8_t HP_6_CloseRate(void);

uint8_t HP_6_GetBpResult(void);
uint8_t HP_6_OpenBp(void);
uint8_t HP_6_CloseBp(void);

uint8_t HP_6_GetADC(uint8_t num, uint8_t id);

uint8_t HP_6_PowerSaving(void);

uint8_t HP_6_VersionInfo(void);

#endif


