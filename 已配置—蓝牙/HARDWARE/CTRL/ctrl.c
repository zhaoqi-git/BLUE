#include "stm32f10x.h"
#include "ctrl.h"
#include "key.h"
#include "servomotor.h"
#include "motor.h"

/*函数名称：FAN_1();
	返回值：void
	函数作用：设置电机PWM，控制风扇转速为1档*/
void FAN_1(void)
{
	TIM_SetCompare1(TIM1,30);
}

/*函数名称：FAN_2();
	返回值：void
	函数作用：设置电机PWM，控制风扇转速为2档*/
void FAN_2(void)
{
	TIM_SetCompare1(TIM1,45);
}

/*函数名称：FAN_3();
	返回值：void
	函数作用：设置电机PWM，控制风扇转速为3档*/
void FAN_3(void)
{
	TIM_SetCompare1(TIM1,55);
}


