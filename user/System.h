#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "stm32f10x.h"
#include "Display.h"

// 初始化所有外设（除了 DHT11）
void System_InitAll(void);

// 温湿度定时读取（非阻塞，在主循环中调用）
void System_ProcessDHT11(void);

// 采集所有传感器数据到 DisplayData_t
void System_CollectData(DisplayData_t *data);

// 按键处理（调节阈值）
void System_HandleKey(void);

// 报警判断与处理（传入当前 ADC 和 MAX）
void System_HandleAlarm(uint16_t adc_value, uint16_t max_limit);

// 页面轮播逻辑（传入报警状态，返回当前页面）
Page_Type System_ProcessPageSwitch(uint8_t alarm_active);

// 串口发送完整数据
void System_SendSerialData(DisplayData_t *data);

// LED 心跳处理（非报警时调用）
void System_HeartbeatProcess(void);

void IWDG_Init(void);
void IWDG_Feed(void);

#endif
