#include "Display.h"
#include "OLED.h"
#include <stdio.h>

static char buf[20];

// 显示光照页面
static void Show_Light_Page(DisplayData_t *data)
{
    sprintf(buf, "Light:%4d", data->adc_value);
    OLED_ShowString(1, 1, buf);
    
    sprintf(buf, "Pct:%3d%%", data->percent);
    OLED_ShowString(2, 1, buf);
    
    sprintf(buf, "MAX:%4d", data->max_limit);
    OLED_ShowString(3, 1, buf);
    
    if(data->alarm_active)
    {
        OLED_ShowString(4, 1, "LOW!           ");
    }
    else
    {
        sprintf(buf, "MIN:%4d", data->min_limit);
        OLED_ShowString(4, 1, buf);
    }
}

// 显示温湿度页面
static void Show_DHT11_Page(DisplayData_t *data)
{
    if(data->dht11_valid)
    {
        sprintf(buf, "Temp:%2dC", data->temp);
        OLED_ShowString(1, 1, buf);
        
        sprintf(buf, "Humi:%2d%%", data->humi);
        OLED_ShowString(2, 1, buf);
    }
    else
    {
        OLED_ShowString(1, 1, "Temp:ERR");
        OLED_ShowString(2, 1, "Humi:ERR");
    }
    
    // 清空未使用的行
    OLED_ShowString(3, 1, "                ");
    OLED_ShowString(4, 1, "                ");
}

void Display_Update(DisplayData_t *data, Page_Type page)
{
    if(page == PAGE_LIGHT)
    {
        Show_Light_Page(data);
    }
    else
    {
        Show_DHT11_Page(data);
    }
}