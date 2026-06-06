#include "Limit.h"
#include "Serial.h"
#include "MyFLASH.h"
#include <stdio.h>

static uint16_t current_max = 2500;

#define FLASH_SAVE_ADDR    0x0800FC00

void Limit_Init(void)
{
    uint32_t saved_value = MyFLASH_ReadWord(FLASH_SAVE_ADDR);

    if(saved_value >= LIMIT_MIN && saved_value <= LIMIT_MAX)
    {
        current_max = (uint16_t)saved_value;
    }
    else
    {
        Set_Limit_Max(current_max);
    }
}

uint16_t Get_Limit_Max(void)
{
    return current_max;
}

void Set_Limit_Max(uint16_t val)
{
    if(val < LIMIT_MIN) val = LIMIT_MIN;
    if(val > LIMIT_MAX) val = LIMIT_MAX;
    
    if(val != current_max)
    {
        uint16_t old = current_max;
        current_max = val;
        
        MyFLASH_ErasePages(FLASH_SAVE_ADDR);
        MyFLASH_ProgramWord(FLASH_SAVE_ADDR, (uint32_t)current_max);
        
        char buf[50];
        sprintf(buf, "Limit saved: MAX %d -> %d\r\n", old, current_max);
        Serial_SendString(buf);
    }
}

uint8_t Key_SetLimit(uint8_t key_num)
{
    uint16_t new_val = current_max;
    
    if(key_num == 1)        new_val += LIMIT_STEP;
    else if(key_num == 2)   new_val -= LIMIT_STEP;
    else return 0;
    
    if(new_val < LIMIT_MIN) new_val = LIMIT_MIN;
    if(new_val > LIMIT_MAX) new_val = LIMIT_MAX;
    
    if(new_val != current_max)
    {
        Set_Limit_Max(new_val);
        return 1;
    }
    return 0;
}

void Save_Limit(void)
{
    MyFLASH_ErasePages(FLASH_SAVE_ADDR);
    MyFLASH_ProgramWord(FLASH_SAVE_ADDR, (uint32_t)current_max);
    
    char buf[40];
    sprintf(buf, "Limit manually saved: MAX %d\r\n", current_max);
    Serial_SendString(buf);
}