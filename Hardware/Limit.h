#ifndef __LIMIT_H
#define __LIMIT_H

#include "stm32f10x.h"

#define LIMIT_MIN   200
#define LIMIT_MAX   3900
#define LIMIT_STEP  50

void Limit_Init(void);
uint16_t Get_Limit_Max(void);
void Set_Limit_Max(uint16_t val);
uint8_t Key_SetLimit(uint8_t key_num);
void Save_Limit(void);

#endif

