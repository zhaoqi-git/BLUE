#include "ds18b20.h"
#include "delay.h"	  


/*�������ƣ�DS18B20_Rst();
	����ֵ���ͣ�void
	�������ã�����������λ���壬��λ18B20*/
/*��������͵�ƽ�����ֵ͵�ƽʱ������480 us���Բ�����λ���塣
	���������ͷ����ߣ�4.7K���������轫���������ߣ���ʱ15��60 us����*/
void DS18B20_Rst(void)	   
{                 
		DS18B20_IO_OUT(); 	//SET PG11 OUTPUT
    DS18B20_DQ_OUT=0; 	//����DQ
    delay_us(750);    	//����750us(����480us)
    DS18B20_DQ_OUT=1; 	//DQ=1 
		delay_us(15);     	//15US
}

/*�������ƣ�DS18B20_Check();
	����ֵ���ͣ�u8
	�������ã��ȴ�DS18B20������Ӧ���ź�  ����1:δ��⵽DS18B20�Ĵ��� ����0:����*/
/*�����������ģʽ(Rx)������DS18B20��������60~240 us���Բ����͵�ƽӦ������*/
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();	//SET PG11 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)//�ȵȴ�200us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;//һֱû�����ͣ�δ�����18B20
	else retry=0;          //������͵�ƽʱ��
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;//����240us��˵����Ч	    
	return 0;              //���� ��Ч
}

/*�������ƣ�DS18B20_Read_Bit();
	����ֵ���ͣ�u8
	�������ã�������DS18B20��ȡһ��λ  ����ֵ��1/0*/
/*��������͵�ƽ��ʱ2us��Ȼ������ת������ģʽ��ʱ12us��
	Ȼ���ȡ�����ߵ�ǰ�ĵ�ƽ��Ȼ����ʱ50us��*/
u8 DS18B20_Read_Bit(void) 	 
{
    u8 data;
		DS18B20_IO_OUT();	//SET PG11 OUTPUT
    DS18B20_DQ_OUT=0; 
		delay_us(2);
    DS18B20_DQ_OUT=1; 
		DS18B20_IO_IN();	//SET PG11 INPUT
		delay_us(12);
		if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}

/*�������ƣ�DS18B20_Read_Byte();
	����ֵ���ͣ�u8
	�������ã�������DS18B20��ȡһ���ֽ�  ����ֵ������������*/
u8 DS18B20_Read_Byte(void)     
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

/*�������ƣ�DS18B20_Write_Byte();
	����ֵ���ͣ�void
	�������ã�����дһ���ֽڵ�DS18B20  dat��Ҫд����ֽ�*/
/*д1ʱ����������͵�ƽ����ʱ2us��Ȼ���ͷ����ߣ���ʱ60us��
	д0ʱ����������͵�ƽ����ʱ60us��Ȼ���ͷ����ߣ���ʱ2us��*/
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
		DS18B20_IO_OUT();	//SET PG11 OUTPUT;
    for (j=1;j<=8;j++) 
		{
        testb=dat&0x01; //�ж���0��1
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;	// Write 1
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;	// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}
 
/*�������ƣ�DS18B20_Start();
	����ֵ���ͣ�void
	�������ã���ʼ�¶�ת��*/ 
void DS18B20_Start(void) 
{   						               
    DS18B20_Rst();	  //��λ  
		DS18B20_Check();	//����Ƿ���ģ��
    DS18B20_Write_Byte(0xcc);	//��SKIP ROM����0xcc skip rom
    DS18B20_Write_Byte(0x44);	//����ʼת������0X44 convert
}

/*�������ƣ�DS18B20_Init();
	����ֵ���ͣ�u8
	�������ã���ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���  ����1:������  ����0:����*/  	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PORTG��ʱ�� 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//PORTG.11 �������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOG,GPIO_Pin_11);    //���1

	DS18B20_Rst();

	return DS18B20_Check();
}  

/*�������ƣ�DS18B20_Get_Temp();
	����ֵ���ͣ�short
	�������ã���ds18b20�õ��¶�ֵ  ���ȣ�0.1C  ����ֵ���¶�ֵ ��-550~1250��*/  
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	short tem;
    DS18B20_Start ();  			// ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	//����SKIP ROM���0XCC��
    DS18B20_Write_Byte(0xbe);	//�����洢�����0XBE��	    
    TL=DS18B20_Read_Byte(); 	//LSB ��ȡ���ֽ�  
    TH=DS18B20_Read_Byte(); 	//MSB ��ȡ���ֽ�
	
	 /*ת����õ���12λ���ݣ��������е�ǰ��5λ�Ƿ���λ���¶ȴ���0�� ��5λΪ0��
	 ֻҪ���⵽����ֵ����0.0625���ɵõ�ʵ���¶ȣ�����¶�С��0����5λΪ1��
	 �⵽����ֵ��Ҫȡ����1�ٳ���0.0625���ɵõ�ʵ�� �¶ȡ�*/  
	
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;					//�¶�Ϊ��  
    }else temp=1;				//�¶�Ϊ��	  	  
    tem=TH; 					//��ø߰�λ
    tem<<=8;    
    tem+=TL;					//��õװ�λ
    tem=(float)tem*0.625;		//ת��     
	if(temp)return tem; 		//�����¶�ֵ
	else return -tem;    
}



 
