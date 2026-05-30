/*
 * motor.c
 *
 *  Created on: May 14, 2026
 *      Author: GB Center
 */

#ifndef MYLIB_INC_MOTOR_C_
#define MYLIB_INC_MOTOR_C_

#include "stm32f407xx.h"

extern volatile int8_t Motor_Left_Dir;
extern volatile int8_t Motor_Right_Dir;
void motor_pwm_init(u8 psc, u16 arr); // TIMER 4
void motor_r_stop();
void motor_l_stop();
void motor_run(i16 left_pwm, i16 right_pwm);
void motor_init(u8 psc, u16 arr);
#endif /* MYLIB_INC_MOTOR_C_ */
