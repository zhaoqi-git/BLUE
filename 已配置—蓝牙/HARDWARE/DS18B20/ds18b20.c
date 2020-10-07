#include "ds18b20.h"
#include "delay.h"	  


/*函数名称：DS18B20_Rst();
	返回值类型：void
	函数作用：主机产生复位脉冲，复位18B20*/
/*主机输出低电平，保持低电平时间至少480 us，以产生复位脉冲。
	接着主机释放总线，4.7K的上拉电阻将单总线拉高，延时15～60 us，。*/
void DS18B20_Rst(void)	   
{                 
		DS18B20_IO_OUT(); 	//SET PG11 OUTPUT
    DS18B20_DQ_OUT=0; 	//拉低DQ
    delay_us(750);    	//拉低750us(至少480us)
    DS18B20_DQ_OUT=1; 	//DQ=1 
		delay_us(15);     	//15US
}

/*函数名称：DS18B20_Check();
	返回值类型：u8
	函数作用：等待DS18B20产生的应答信号  返回1:未检测到DS18B20的存在 返回0:存在*/
/*主机进入接收模式(Rx)，接着DS18B20拉低总线60~240 us，以产生低电平应答脉冲*/
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();	//SET PG11 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)//先等待200us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;//一直没有拉低，未检出到18B20
	else retry=0;          //否则检测低电平时间
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;//大于240us，说明无效	    
	return 0;              //否则 有效
}

/*函数名称：DS18B20_Read_Bit();
	返回值类型：u8
	函数作用：主机从DS18B20读取一个位  返回值：1/0*/
/*主机输出低电平延时2us，然后主机转入输入模式延时12us，
	然后读取单总线当前的电平，然后延时50us。*/
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

/*函数名称：DS18B20_Read_Byte();
	返回值类型：u8
	函数作用：主机从DS18B20读取一个字节  返回值：读到的数据*/
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

/*函数名称：DS18B20_Write_Byte();
	返回值类型：void
	函数作用：主机写一个字节到DS18B20  dat：要写入的字节*/
/*写1时序：主机输出低电平，延时2us，然后释放总线，延时60us。
	写0时序：主机输出低电平，延时60us，然后释放总线，延时2us。*/
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
		DS18B20_IO_OUT();	//SET PG11 OUTPUT;
    for (j=1;j<=8;j++) 
		{
        testb=dat&0x01; //判断是0是1
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
 
/*函数名称：DS18B20_Start();
	返回值类型：void
	函数作用：开始温度转换*/ 
void DS18B20_Start(void) 
{   						               
    DS18B20_Rst();	  //复位  
		DS18B20_Check();	//检测是否有模块
    DS18B20_Write_Byte(0xcc);	//发SKIP ROM命令0xcc skip rom
    DS18B20_Write_Byte(0x44);	//发开始转换命令0X44 convert
}

/*函数名称：DS18B20_Init();
	返回值类型：u8
	函数作用：初始化DS18B20的IO口 DQ 同时检测DS的存在  返回1:不存在  返回0:存在*/  	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //使能PORTG口时钟 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//PORTG.11 推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOG,GPIO_Pin_11);    //输出1

	DS18B20_Rst();

	return DS18B20_Check();
}  

/*函数名称：DS18B20_Get_Temp();
	返回值类型：short
	函数作用：从ds18b20得到温度值  精度：0.1C  返回值：温度值 （-550~1250）*/  
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	short tem;
    DS18B20_Start ();  			// ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	//发送SKIP ROM命令（0XCC）
    DS18B20_Write_Byte(0xbe);	//发读存储器命令（0XBE）	    
    TL=DS18B20_Read_Byte(); 	//LSB 读取低字节  
    TH=DS18B20_Read_Byte(); 	//MSB 读取高字节
	
	 /*转化后得到的12位数据，二进制中的前面5位是符号位，温度大于0， 这5位为0，
	 只要将测到的数值乘于0.0625即可得到实际温度；如果温度小于0，这5位为1，
	 测到的数值需要取反加1再乘于0.0625即可得到实际 温度。*/  
	
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;					//温度为负  
    }else temp=1;				//温度为正	  	  
    tem=TH; 					//获得高八位
    tem<<=8;    
    tem+=TL;					//获得底八位
    tem=(float)tem*0.625;		//转换     
	if(temp)return tem; 		//返回温度值
	else return -tem;    
}



 
