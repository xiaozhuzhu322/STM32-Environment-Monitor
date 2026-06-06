#include "stm32f10x.h"
#include "Delay.h"
#include "Serial.h"
#include "dht11.h"
#include "Alarm.h"
#include "System.h"
#include "Display.h"

int main(void)
{
    DisplayData_t display_data;
    Page_Type current_page;
    
    // ========== 1. 初始化所有硬件 ==========
    System_InitAll();
    
    // ========== 2. 独立看门狗初始化（放在所有初始化之后）==========
    IWDG_Init();   // 初始化看门狗，超时时间1秒
    
    // ========== 3. DHT11 单独初始化（需要等待）==========
    while(DHT11_Init())
    {
        Serial_SendString("DHT11 Init Error!\r\n");
        Delay_ms(1000);
        IWDG_Feed();   // 等待期间也要喂狗，防止复位
    }
    Serial_SendString("System Start!\r\n");
    
    // ========== 4. 主循环 ==========
    while(1)
    {
        // 1. 定时读取温湿度
        System_ProcessDHT11();
        
        // 2. 采集所有数据
        System_CollectData(&display_data);
        
        // 3. 按键处理
        System_HandleKey();
        
        // 4. 报警判断
        System_HandleAlarm(display_data.adc_value, display_data.max_limit);
        
        // 5. 页面轮播（获取当前应该显示的页面）
        current_page = System_ProcessPageSwitch(Alarm_IsActive());
        
        // 6. LED 控制
        if(Alarm_IsActive())
            Alarm_Process();
        else
            System_HeartbeatProcess();
        
        // 7. OLED 显示
        Display_Update(&display_data, current_page);
        
        // 8. 串口发送
        System_SendSerialData(&display_data);
        
        // 9. 喂狗（主循环结束前喂一次，告诉看门狗系统正常）
        IWDG_Feed();
        
        Delay_ms(500);
    }
}