#ifndef __SERVOMOTOR_H
#define __SERVOMOTOR_H

#include "stm32f10x.h"

//ͨ���ı�TIM3->CCR1��ֵ���ı�ռ�ձȣ��Ӷ����ƶ���ĽǶ�
#define SERVOMOTOR_PWM_VAL TIM3->CCR1    

void SERVOMOTOR_PWM_Init(u16 arr,u16 psc);      //�����ʼ��


#endif



