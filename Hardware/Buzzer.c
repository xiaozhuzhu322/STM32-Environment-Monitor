#include "stm32f10x.h"                  // Device header


void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启GPIOB时钟
    GPIO_InitTypeDef GPIO_InitStructer;//定义结构体
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_12;//PB12端口
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructer);//写入配置
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//初始化蜂鸣器，置1，这样刚开始就不会响
}


void Buzzer_ON(void)//蜂鸣器开函数
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//置0，低电平，开蜂鸣器
}
void BUZZER_TURN(void)//翻转函数
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	}
	else
	{
	    GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	}
	
}

void Buzzer_OFF(void)//关
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//置1，高电平，关蜂鸣器
}
