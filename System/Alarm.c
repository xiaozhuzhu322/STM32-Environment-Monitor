#include "Alarm.h"
#include "Delay.h"
#include "stm32f10x.h"

// LED 引脚（PC13）
#define LED_PORT    GPIOC
#define LED_PIN     GPIO_Pin_13

// 蜂鸣器引脚（PB13）
#define BEEP_PORT   GPIOB
#define BEEP_PIN    GPIO_Pin_13

static uint8_t alarm_active = 0;      // 0=正常，1=报警中
static uint32_t last_tick = 0;        // 用于闪烁计时
static uint8_t led_state = 0;         // 当前LED状态（0灭1亮）

// 初始化 LED 和蜂鸣器
void Alarm_Init(void)
{
    // 开启 GPIO 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // 初始化 PC13 LED（推挽输出）
    GPIO_InitStruct.GPIO_Pin = LED_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStruct);
    GPIO_WriteBit(LED_PORT, LED_PIN, Bit_SET);  // 初始熄灭
    
    // 初始化 PB13 蜂鸣器（推挽输出）
    GPIO_InitStruct.GPIO_Pin = BEEP_PIN;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_WriteBit(BEEP_PORT, BEEP_PIN, Bit_SET);  // 初始关闭（假设高电平关闭）
}

// 开启报警
void Alarm_Start(void)
{
    if(alarm_active == 0)
    {
        alarm_active = 1;
        last_tick = 0;      // 重置计时，让闪烁立即开始
        led_state = 0;
    }
}

// 停止报警
void Alarm_Stop(void)
{
    if(alarm_active == 1)
    {
        alarm_active = 0;
        // 关闭 LED 和蜂鸣器
        GPIO_WriteBit(LED_PORT, LED_PIN, Bit_SET);
        GPIO_WriteBit(BEEP_PORT, BEEP_PIN, Bit_SET);
    }
}

// 获取报警状态
uint8_t Alarm_IsActive(void)
{
    return alarm_active;
}

// 报警处理（需要在主循环中频繁调用，比如每50ms一次）
// 返回值：1=报警中，0=正常
uint8_t Alarm_Process(void)
{
    static uint32_t tick_counter = 0;
    static uint8_t beep_state = 0;
    
    if(!alarm_active)
    {
        // 正常模式：LED 心跳（慢闪，1秒亮1秒灭）
        // 这里简化处理，由主函数自己控制心跳，或者你也可以在这里实现
        return 0;
    }
    
    // 报警模式：LED 和蜂鸣器同步快速闪烁（每200ms切换一次）
    tick_counter++;
    if(tick_counter >= 4)  // 假设主循环每50ms调用一次，4次=200ms
    {
        tick_counter = 0;
        led_state = !led_state;
        
        // 控制 LED
        GPIO_WriteBit(LED_PORT, LED_PIN, led_state ? Bit_RESET : Bit_SET);
        
        // 控制蜂鸣器（同步）
        GPIO_WriteBit(BEEP_PORT, BEEP_PIN, led_state ? Bit_RESET : Bit_SET);
    }
    
    return 1;
}