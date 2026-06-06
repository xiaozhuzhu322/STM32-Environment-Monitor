#include "LightSensor_ADC.h"
#include "AD.h"

#define LIGHT_MIN_VALUE   500
#define LIGHT_MAX_VALUE   3900

static uint16_t filtered_adc = 0;
static uint8_t first_read = 1;

void LightSensor_ADC_Init(void)
{
    AD_Init();
}

uint16_t LightSensor_GetRawFiltered(void)
{
    uint16_t raw_adc = AD_GetValue();
    
    if(first_read)
    {
        filtered_adc = raw_adc;
        first_read = 0;
    }
    else
    {
        // 一阶低通滤波，使变化平滑
        filtered_adc = (uint16_t)((uint32_t)filtered_adc * 97 / 100 + (uint32_t)raw_adc * 3 / 100);
    }
    
    return filtered_adc;
}

uint8_t LightSensor_GetPercent(void)
{
    uint16_t adc = LightSensor_GetRawFiltered();
    
    // 限制范围
    if(adc < LIGHT_MIN_VALUE) adc = LIGHT_MIN_VALUE;
    if(adc > LIGHT_MAX_VALUE) adc = LIGHT_MAX_VALUE;
    
    // 越亮（ADC越小）→ 百分比越大
    return (uint8_t)((LIGHT_MAX_VALUE - adc) * 100 / (LIGHT_MAX_VALUE - LIGHT_MIN_VALUE));
}