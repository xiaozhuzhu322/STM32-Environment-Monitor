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
    GPIO_WriteBit(BEEP_PORT, BEEP_PIN, Bit_SET);  // 初始关闭
}

// 开启报警
void Alarm_Start(void)
{
    if(alarm_active == 0)
    {
        alarm_active = 1;
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

// 报警处理：LED 和蜂鸣器快速闪烁（100ms 切换一次）
uint8_t Alarm_Process(void)
{
    static uint8_t blink_state = 0;
    
    if(!alarm_active)
        return 0;
    
    // 快速闪烁：每 100ms 切换一次状态
    blink_state = !blink_state;
    
    // 控制 LED（低电平点亮）
    GPIO_WriteBit(LED_PORT, LED_PIN, blink_state ? Bit_RESET : Bit_SET);
    
    // 控制蜂鸣器（同步）
    GPIO_WriteBit(BEEP_PORT, BEEP_PIN, blink_state ? Bit_RESET : Bit_SET);
    
    // 延时 100ms，实现快速闪烁效果
    Delay_ms(100);
    
    return 1;
}