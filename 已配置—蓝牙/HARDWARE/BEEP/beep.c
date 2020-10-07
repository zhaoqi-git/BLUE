#include "beep.h"
   

/*函数名称：BEEP_Init();
	返回值：void
	函数作用：蜂鸣器初始化 初始化PA8为输出口.并使能这个口的时钟*/
void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能GPIOA端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //BEEP-->PA.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);	 //根据参数初始化GPIOA.8
 
 GPIO_ResetBits(GPIOA,GPIO_Pin_8);//输出0，关闭蜂鸣器输出

}

/*函数名称：ALARM_Init();
	返回值：void
	函数作用：火焰传感器D0口初始化 初始化PE13为输入口.并使能这个口的时钟
						PE13为电平为高，说明没有火焰；电平为低，说明有火焰*/
void ALARM_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能GPIOE端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置为上拉输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);	 //根据参数初始化GPIOE.13
}
