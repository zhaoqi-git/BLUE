#ifndef __OLED_H
#define __OLED_H		

#include "sys.h"
#include "stdlib.h"	   

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//SSD1306 OLED 驱动IC驱动代码
//驱动方式:8080并口/4线串口
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

// Modified by Phantom
  
//---------------------------OLED端口定义--------------------------  

//使用4线串行接口时使用 
#define OLED_SCLK PEout(6) //D0
#define OLED_SDIN PEout(5) //D1
#define OLED_RST PEout(4)  //RES	
#define OLED_RS  PEout(3)  //DC

//PC0~7,作为数据线
#define DATAOUT(x) GPIO_Write(GPIOB,x);//输出  

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);  		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);	

void OLED_Show_Font(u16 x,u16 y,u8 fnum,u8 size);	 


#endif  
	 
