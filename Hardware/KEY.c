#include "stm32f10x.h"
#include "Delay.h"

static uint8_t last_key_state = 0;  // 上一次按键状态

void KEY_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructer.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
    GPIO_InitStructer.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructer);
}

uint8_t KEY_GetNum(void)
{
    uint8_t current_key = 0;
    
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) current_key = 1;
    else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0) current_key = 2;
    
    // 检测按下瞬间（边沿）
    if(current_key != 0 && last_key_state == 0)
    {
        Delay_ms(20);  // 消抖
        last_key_state = current_key;
        return current_key;
    }
    
    // 检测松手
    if(current_key == 0 && last_key_state != 0)
    {
        last_key_state = 0;
    }
    
    return 0;
}