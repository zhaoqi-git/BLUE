#include "stm32f10x.h"
#include "ctrl.h"
#include "key.h"
#include "servomotor.h"
#include "motor.h"

/*�������ƣ�FAN_1();
	����ֵ��void
	�������ã����õ��PWM�����Ʒ���ת��Ϊ1��*/
void FAN_1(void)
{
	TIM_SetCompare1(TIM1,30);
}

/*�������ƣ�FAN_2();
	����ֵ��void
	�������ã����õ��PWM�����Ʒ���ת��Ϊ2��*/
void FAN_2(void)
{
	TIM_SetCompare1(TIM1,45);
}

/*�������ƣ�FAN_3();
	����ֵ��void
	�������ã����õ��PWM�����Ʒ���ת��Ϊ3��*/
void FAN_3(void)
{
	TIM_SetCompare1(TIM1,55);
}


