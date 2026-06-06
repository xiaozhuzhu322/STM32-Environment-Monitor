#include "stm32f10x.h"                  // Device header
void LED_Init(void)//初始化函数
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启GPIOA时钟，给GPIOA上电
	GPIO_InitTypeDef GPIO_InitStructer;//定义GPIO初始化结构体，存放GPIO的配置
    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;//设置为推挽输出，强高电平，强低电平
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_2;//端口一和二
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);//配置写入硬件寄存器
	
}

void LED1_ON(void)//第一个的灯点亮函数
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//低电平亮
}
void LED1_TURN(void)//第一个灯翻转
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)//GPIO_ReadInputDataBit()读取当前引脚电平，低变高（亮变灭），高变低（灭变亮）
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
	else
	{
	    GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}
	
}

void LED1_OFF(void)//第一个灯熄灭函数
{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);//高电平灭
}
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
void LED2_TURN(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
	}
	else
	{
	    GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	}
	
}
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
}

