#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "stm32f10x.h"

// 页面枚举
typedef enum
{
    PAGE_LIGHT = 0,
    PAGE_DHT11 = 1
} Page_Type;

// 显示数据参数结构体
typedef struct
{
    uint16_t adc_value;
    uint8_t percent;
    uint16_t max_limit;
    uint16_t min_limit;
    uint8_t temp;
    uint8_t humi;
    uint8_t dht11_valid;
    uint8_t alarm_active;
} DisplayData_t;

void Display_Update(DisplayData_t *data, Page_Type page);

#endif
