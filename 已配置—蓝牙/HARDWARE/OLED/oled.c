#include "OLED.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//SSD1306 OLED ����IC��������
//������ʽ:8080����/4�ߴ���
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

// Modified By Phantom 2016-08-29

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
u8 OLED_GRAM[128][8];	 

/*************************************************************************
*
*  �������ƣ�OLED_Init
*  ����˵������ʼ��OLED
*  �޸�ʱ�䣺2016-08-29   �޸ı�ע �Ѳ���
*  
*************************************************************************/

void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_2;	 //PD3,PD6�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOE, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
// 	GPIO_SetBits(GPIOE,GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_2);	//PD3,PD6 �����	 
	
	OLED_RST=0;
	delay_ms(100);
	OLED_RST=1;
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 
	OLED_Clear();
}  


//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}


/*************************************************************************
*
*  �������ƣ�OLED_WrDat(uint8 data)
*  ����˵����OLEDд����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-12-12   �Ѳ���
*  
*************************************************************************/

void OLED_WrDat(uint8_t data)
{
  uint8_t i=8;
  
  OLED_RST = OLED_DATA;
  //GPIOA_PDOR |=  GPIO_PDOR_PDO(OLED_DC);
  //     asm("nop"); 
  
  OLED_SCLK = OLED_CMD;
  //GPIOA_PDOR &= ~GPIO_PDOR_PDO(OLED_SCL);
  //      asm("nop");    
  
  while(i--)
  {
    if(data&0x80){OLED_SDIN = OLED_DATA;}   //{GPIOA_PDOR |=  GPIO_PDOR_PDO(OLED_SDA);}
    else{OLED_SDIN = OLED_CMD;}             //{GPIOA_PDOR &= ~GPIO_PDOR_PDO(OLED_SDA);}
    OLED_SCLK = OLED_DATA;
    //asm("nop");           
    OLED_SCLK = OLED_CMD;   
    data<<=1;    
  }
}


/*************************************************************************
*
*  �������ƣ�OLED_WR_Byte(uint8 data)
*  ����˵����OLEDд�ֽ�
*  ����˵������
*  �޸�ʱ�䣺2016-08-29   �Ѳ���
*  
*************************************************************************/

void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	OLED_RS=cmd; //д���� 
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK=0;
		if(dat&0x80)OLED_SDIN=1;
		else OLED_SDIN=0;
		OLED_SCLK=1;
		dat<<=1;   
	}				 
	OLED_RS=1;   	  
}
	  	  
/*************************************************************************
*
*  �������ƣ�OLED_Display_On
*  ����˵��������OLED��ʾ
*  ����˵������
*  �޸�ʱ�䣺2016-08-29   �Ѳ���
*  
*************************************************************************/

void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/*************************************************************************
*
*  �������ƣ�OLED_Display_Off
*  ����˵�����ر�OLED��ʾ 
*  ����˵������
*  �޸�ʱ�䣺2016-08-29   �Ѳ���
*  
*************************************************************************/

void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   		

/*************************************************************************
*
*  �������ƣ�OLED_Clear
*  ����˵��������
*  ����˵������
*  �޸�ʱ�䣺2016-08-29   �Ѳ���
*  
*************************************************************************/

void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}

/*************************************************************************
*
*  �������ƣ�OLED_DrawPoint
*  ����˵�������㺯��
*  ����˵����x:0~127  y:0~63  t:1 ��� 0,���	
*  �޸�ʱ�䣺2016-08-29   �Ѳ���
*  
*************************************************************************/
	   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)   //������Χ��.
  {
    pos=7-y/8;
    bx=y%8;
    temp=1<<(7-bx);
    if(t)OLED_GRAM[x-31][pos]|=temp;
    else OLED_GRAM[x-31][pos]&=~temp;	
  }
  else
  {
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
  }
}

/*************************************************************************
*
*  �������ƣ�OLED_DrawPoint
*  ����˵����x1,y1,x2,y2 �������ĶԽ�����
*  ����˵����x:0~127  y:0~63  t:1 ��� 0,���	ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
*  �޸�ʱ�䣺2016-08-29   �Ѳ���
*  
*************************************************************************/
  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}

/*************************************************************************
*
*  �������ƣ�OLED_ShowChar
*  ����˵������ָ��λ����ʾһ���ַ�,���������ַ�
*  ����˵����x:0~127  y:0~63   size:ѡ������ 12/16/24   mode:0,������ʾ;1,������ʾ	
*  �޸�ʱ�䣺2016-08-29   �Ѳ���
*  
*************************************************************************/

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}


//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

/*************************************************************************
*
*  �������ƣ�OLED_ShowNum
*  ����˵������ʾ2������
*  ����˵����x:0~127  y:0~63  num:��ֵ(0~4294967295) len :���ֵ�λ��   0,���ģʽ;1,����ģʽ  	size:�����С
*  �޸�ʱ�䣺2016-08-29   �Ѳ���
*  
*************************************************************************/
 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 


/*************************************************************************
*
*  �������ƣ�OLED_ShowString
*  ����˵������ʾ�ַ���
*  ����˵������ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��63��*p:�ַ�����ʼ��ַ�� size:�����С��12,16,24����
*  �޸�ʱ�䣺2016-08-29   �޸ı�ע �Ѳ���
*  
*************************************************************************/

void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size))
          {
            //OLED_ShowChar(x,y-64+size,*p,size,1);
            //OLED_ShowChar(x,y,*p,size,1);
          }
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	
}	   

/*************************************************************************
*
*  �������ƣ�void Draw_BMP(uint8_t x,uint8_t y)
*  ����˵������ʾBMPͼƬ
*  ����˵������ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
*  �������أ���
*  �޸�ʱ�䣺2012-12-12   �޸ı�ע �Ѳ���
*  
*************************************************************************/


void Draw_BMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[])
{
  uint16_t ii=0;
  uint8_t x,y;

  if(y1%8==0) y=y1/8;
  else y=y1/8+1;
	for(y=y0;y<=y1;y++)
	{
          OLED_DrawPoint(x0,y,1);
          for(x=x0;x<x1;x++)
	    {
	    	OLED_WrDat(bmp[ii++]);
	    }
	}
}

//��ָ��λ�ã���ʾһ������
//x,y :���ֵ�����
//fnum:���ֱ�ţ����ֿ���������ı�ţ� ��size:�����С��10,16��

void OLED_Show_Font(u16 x,u16 y,u8 fnum,u8 size)
{
	u8 temp,t,t1;
	u16 y0=y;
	u8 *dzk;  
  u8 csize =0;
  if(size==10) 
	{ 
		csize=20;
    dzk=(u8*)OLED_HZK_TBL[fnum];
	}
  if(size==16) 
	{ 
		csize=32;
	  dzk=(u8*)OLED_HZK_TBL0[fnum];
	}//�õ����ֱ�Ŷ�Ӧ�ĵ���� 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];				//�õ���������                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,1);
			else OLED_DrawPoint(x,y,0); 
			temp<<=1;
			y++;
			if((y-y0)==(csize/2))
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}
