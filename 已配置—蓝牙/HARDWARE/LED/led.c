#include "led.h"

/*函数名称：LED_Init();
	返回值：void
	函数作用：LED灯初始化 初始化PB11、PB12、PB13为输出口.并使能这个口的时钟*/
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 //使能PB端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_11);						 //PB.11 输出高
	GPIO_SetBits(GPIOB,GPIO_Pin_12);						 //PB.12 输出高
	GPIO_SetBits(GPIOB,GPIO_Pin_13);						 //PB.13 输出高

}
 
