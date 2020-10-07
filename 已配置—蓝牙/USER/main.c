#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
#include "usart1.h"			 	 
#include "string.h"	   	
 
/************************************************
 ALIENTEK 战舰STM32F103开发板实验12
 OLED显示实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
extern u8 USART3_RX_BUF[USART1_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
int main(void)
{
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	LED_Init();				//初始化与LED连接的硬件接口
	KEY_Init();				//初始化按键
  OLED_Init();				//初始化LCD		
  usart1_init(115200);

	while(1)
	{	
		if(USART1_RX_STA&0X8000)			//接收到一次数据了
		{		
			if(USART1_RX_BUF[0]=='W')
			{	
				//LED0=0;
				delay_ms(1000);
				//LED0=1;
 			  OLED_ShowString(30,35,"LED0",12);	
				OLED_ShowNum(0,20,USART1_RX_BUF[0],2,12);
			  OLED_Refresh_Gram();//更新显示	
			}
				
	    else if(USART1_RX_BUF[0]=='A')
			{					
				//LED1=0;
				delay_ms(1000);
			  //LED1=1;
 			  OLED_ShowString(30,35,"LED1",12);
        OLED_ShowNum(0,20,USART1_RX_BUF[0],2,12);				
			  OLED_Refresh_Gram();//更新显示 
			}
				
	    else if(USART1_RX_BUF[0]=='D')
			{
 			  OLED_ShowString(30,35,"OLED",12);	
				OLED_ShowNum(0,20,USART1_RX_BUF[0],2,12);
			  OLED_Refresh_Gram();//更新显示	 
			}	
      USART1_RX_STA=0;					
	}		
	}
}


