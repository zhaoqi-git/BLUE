#ifndef __SERVOMOTOR_H
#define __SERVOMOTOR_H

#include "stm32f10x.h"

//通过改变TIM3->CCR1的值来改变占空比，从而控制舵机的角度
#define SERVOMOTOR_PWM_VAL TIM3->CCR1    

void SERVOMOTOR_PWM_Init(u16 arr,u16 psc);      //舵机初始化


#endif



