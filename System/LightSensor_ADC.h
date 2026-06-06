#ifndef __LIGHT_SENSOR_ADC_H
#define __LIGHT_SENSOR_ADC_H

#include "stm32f10x.h"

void LightSensor_ADC_Init(void);
uint16_t LightSensor_GetRawFiltered(void);  // 滤波后的ADC值
uint8_t LightSensor_GetPercent(void);       // 百分比（0~100，越亮越大）

#endif
