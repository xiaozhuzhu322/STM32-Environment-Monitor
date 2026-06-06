#ifndef __ALARM_H
#define __ALARM_H

#include "stm32f10x.h"

// 初始化报警模块（LED和蜂鸣器的GPIO）
void Alarm_Init(void);

// 启动报警（LED快闪 + 蜂鸣器响）
void Alarm_Start(void);

// 停止报警（恢复心跳模式，蜂鸣器停）
void Alarm_Stop(void);

// 报警状态下需要在主循环中调用的处理（实现闪烁效果）
// 返回值：1=报警中，0=正常
uint8_t Alarm_Process(void);

// 获取当前报警状态
uint8_t Alarm_IsActive(void);

#endif

