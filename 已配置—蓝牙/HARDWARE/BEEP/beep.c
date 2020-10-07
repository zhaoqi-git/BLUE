#include "beep.h"
   

/*�������ƣ�BEEP_Init();
	����ֵ��void
	�������ã���������ʼ�� ��ʼ��PA8Ϊ�����.��ʹ������ڵ�ʱ��*/
void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��GPIOA�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //BEEP-->PA.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);	 //���ݲ�����ʼ��GPIOA.8
 
 GPIO_ResetBits(GPIOA,GPIO_Pin_8);//���0���رշ��������

}

/*�������ƣ�ALARM_Init();
	����ֵ��void
	�������ã����洫����D0�ڳ�ʼ�� ��ʼ��PE13Ϊ�����.��ʹ������ڵ�ʱ��
						PE13Ϊ��ƽΪ�ߣ�˵��û�л��棻��ƽΪ�ͣ�˵���л���*/
void ALARM_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��GPIOE�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //����Ϊ��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);	 //���ݲ�����ʼ��GPIOE.13
}
