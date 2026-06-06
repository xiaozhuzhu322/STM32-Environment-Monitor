#include "stm32f10x.h"                  // Device header

void LightSensor_Init(void)//初始化光照传感器引脚
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//PB13
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t LightSensor_Get()//获取当前引脚电平函数（返回值0/1）
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);//返回PB13寄存器状态
}

