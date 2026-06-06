#include "System.h"
#include "Delay.h"
#include "Serial.h"
#include "LED.h"
#include "LightSensor_ADC.h"
#include "KEY.h"
#include "Limit.h"
#include "Alarm.h"
#include "dht11.h"
#include "OLED.h"
#include <stdio.h>

// 静态变量（仅在本文件内使用）
static uint8_t temp = 0, humi = 0;
static uint8_t dht11_valid = 0;
static uint32_t dht11_timer = 0;
static uint32_t page_timer = 0;
static Page_Type current_page = PAGE_LIGHT;
static uint8_t auto_page_enable = 1;

// 初始化所有外设（DHT11 单独初始化，因为需要等待）
void System_InitAll(void)
{
    Delay_ms(100);
    OLED_Init();
    Serial_Init();
    LED_Init();
    LightSensor_ADC_Init();
    KEY_Init();
    Limit_Init();
    Alarm_Init();
}

// 温湿度定时读取（非阻塞，每2秒读一次）
void System_ProcessDHT11(void)
{
    dht11_timer++;
    if(dht11_timer >= 40)  // 50ms * 40 = 2秒
    {
        dht11_timer = 0;
        if(DHT11_Read_Data(&temp, &humi) == 0)
            dht11_valid = 1;
        else
            dht11_valid = 0;
    }
}

// 采集所有传感器数据
void System_CollectData(DisplayData_t *data)
{
    data->adc_value = LightSensor_GetRawFiltered();
    data->percent = LightSensor_GetPercent();
    data->max_limit = Get_Limit_Max();
    data->min_limit = LIMIT_MIN;
    data->temp = temp;
    data->humi = humi;
    data->dht11_valid = dht11_valid;
    data->alarm_active = Alarm_IsActive();
}

// 按键处理（调节阈值）
void System_HandleKey(void)
{
    uint8_t key_num = KEY_GetNum();
    if(key_num != 0)
    {
        Key_SetLimit(key_num);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
        Delay_ms(50);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
    }
}

// 报警判断与处理
void System_HandleAlarm(uint16_t adc_value, uint16_t max_limit)
{
    if(adc_value > max_limit)
    {
        if(!Alarm_IsActive())
        {
            Alarm_Start();
            Serial_SendString("Warning: Light too low!\r\n");
        }
    }
    else
    {
        if(Alarm_IsActive())
        {
            Alarm_Stop();
            Serial_SendString("Light restored.\r\n");
        }
    }
}

// 页面轮播逻辑（返回当前应该显示的页面）
Page_Type System_ProcessPageSwitch(uint8_t alarm_active)
{
    // 报警时强制切回光照页，禁用自动轮播
    if(alarm_active)
    {
        if(current_page != PAGE_LIGHT)
        {
            current_page = PAGE_LIGHT;
            OLED_Clear();
        }
        auto_page_enable = 0;
        return current_page;
    }
    
    // 恢复正常轮播
    auto_page_enable = 1;
    
    // 自动轮播
    page_timer++;
    if(page_timer >= 40)  // 2秒切换一次
    {
        page_timer = 0;
        current_page = (current_page == PAGE_LIGHT) ? PAGE_DHT11 : PAGE_LIGHT;
        OLED_Clear();
    }
    
    return current_page;
}

// 串口发送完整数据
void System_SendSerialData(DisplayData_t *data)
{
    char buf[80];
    
    if(data->dht11_valid)
    {
        sprintf(buf, "Light:%4d  Pct:%3d%%  MAX:%4d  MIN:%4d   T:%2dC  H:%2d%% StatusL:%s\r\n",
                data->adc_value, data->percent, data->max_limit, data->min_limit,
                data->temp, data->humi, data->alarm_active ? "ALARM" : "NORMAL");
    }
    else
    {
        sprintf(buf, "Light:%4d  Pct:%3d%%  MAX:%4d  MIN:%4d   T:--C  H:--%% StatusL:%s\r\n",
                data->adc_value, data->percent, data->max_limit, data->min_limit,
                data->alarm_active ? "ALARM" : "NORMAL");
    }
    Serial_SendString(buf);
}

// LED 心跳处理（非报警时调用）
void System_HeartbeatProcess(void)
{
    static uint32_t tick = 0;
    static uint8_t state = 0;
    tick++;
    if(tick >= 20)  // 50ms * 20 = 1000ms
    {
        tick = 0;
        state = !state;
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, state ? Bit_RESET : Bit_SET);
    }
}

// ==================== 独立看门狗 (IWDG) ====================
// 初始化独立看门狗
// 超时时间 ≈ 1 秒 (prer=4, rlr=625)
void IWDG_Init(void)
{
    // 1. 使能对寄存器 PR 和 RLR 的写操作
    IWDG->KR = 0x5555;
    
    // 2. 设置分频系数 (prer=4 对应 64分频，IWDG时钟 ≈ 40KHz / 64 = 625Hz)
    IWDG->PR = 4;           // 分频值: 0-7, 4 = 64分频
    
    // 3. 设置重装载值 (rlr=625, 超时时间 = 625 / 625Hz = 1秒)
    IWDG->RLR = 625;
    
    // 4. 重装载计数器
    IWDG->KR = 0xAAAA;
    
    // 5. 启动看门狗
    IWDG->KR = 0xCCCC;
    
    Serial_SendString("IWDG Init OK, Timeout=1s\r\n");
}

// 喂狗（重装载计数器）
void IWDG_Feed(void)
{
    IWDG->KR = 0xAAAA;
}

