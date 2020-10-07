#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
#include "usart1.h"			 	 
#include "string.h"	   	
 
/************************************************
 ALIENTEK ս��STM32F103������ʵ��12
 OLED��ʾʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
extern u8 USART3_RX_BUF[USART1_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
int main(void)
{
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LED_Init();				//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//��ʼ������
  OLED_Init();				//��ʼ��LCD		
  usart1_init(115200);

	while(1)
	{	
		if(USART1_RX_STA&0X8000)			//���յ�һ��������
		{		
			if(USART1_RX_BUF[0]=='W')
			{	
				//LED0=0;
				delay_ms(1000);
				//LED0=1;
 			  OLED_ShowString(30,35,"LED0",12);	
				OLED_ShowNum(0,20,USART1_RX_BUF[0],2,12);
			  OLED_Refresh_Gram();//������ʾ	
			}
				
	    else if(USART1_RX_BUF[0]=='A')
			{					
				//LED1=0;
				delay_ms(1000);
			  //LED1=1;
 			  OLED_ShowString(30,35,"LED1",12);
        OLED_ShowNum(0,20,USART1_RX_BUF[0],2,12);				
			  OLED_Refresh_Gram();//������ʾ 
			}
				
	    else if(USART1_RX_BUF[0]=='D')
			{
 			  OLED_ShowString(30,35,"OLED",12);	
				OLED_ShowNum(0,20,USART1_RX_BUF[0],2,12);
			  OLED_Refresh_Gram();//������ʾ	 
			}	
      USART1_RX_STA=0;					
	}		
	}
}


