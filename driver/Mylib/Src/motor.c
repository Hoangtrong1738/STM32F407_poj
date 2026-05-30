/*
 * motor.c
 *
 *  Created on: May 14, 2026
 *      Author: GB Center
 */
#include "motor.h"
#include "math.h"
volatile int8_t Motor_Left_Dir  = 0;
volatile int8_t Motor_Right_Dir = 0;
void motor_pwm_init(u8 psc, u16 arr)// TIMER 4
{
	GPIOD_PCLK_EN();
	GPIO_Handle_t D;
	D.pGPIOx =GPIOD;
	D.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFM;
	D.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	D.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	D.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	D.GPIO_PinConfig.GPIO_PinAltFunMode = 2;

	D.GPIO_PinConfig.GPIO_PinNumber = PIN_12;
	GPIO_Init(&D);

	D.GPIO_PinConfig.GPIO_PinNumber = PIN_13;
	GPIO_Init(&D);

	D.GPIO_PinConfig.GPIO_PinNumber = PIN_14;
	GPIO_Init(&D);

	D.GPIO_PinConfig.GPIO_PinNumber = PIN_15;
	GPIO_Init(&D);

	TIM4_PCLK_EN();

	TIM4->PSC = psc-1;
	TIM4->ARR = arr -1;
	TIM4->CNT = 0;
	TIM4->CCR1 = 0;
	TIM4->CCR2 = 0;
	TIM4->CCR3 = 0;
	TIM4->CCR4 = 0;

	TIM4->CCMR1 &= ~(0b11 << 0); //CC1 channel 1 is configured as output compare
	TIM4->CCMR1 &= ~(0B111 << 4);
	TIM4->CCMR1 |= (0B110 << 4);//set PWM mode 1 for compare channel 1


	TIM4->CCMR1 &= ~(0b11 << 8); //CC2 channel 2 is configured as output compare
	TIM4->CCMR1 &= ~(0b111 << 12);
	TIM4->CCMR1 |= (0b110 << 12);//set PWM mode 2


	TIM4->CCMR2 &= ~(0b11 << 0); //CC1 channel 3 is configured as output compare
	TIM4->CCMR2 &= ~(0B111 << 4);
	TIM4->CCMR2 |= (0B110 << 4);//set PWM mode 3 for compare channel 1


	TIM4->CCMR2 &= ~(0b11 << 8); //CC2 channel 4 is configured as output compare
	TIM4->CCMR2 &= ~(0b111 << 12);
	TIM4->CCMR2 |= (0b110 << 12);//set PWM mode 4


	TIM4->CR1 |= (1 << 7); // Auto - reload preload enable

	//TIM4->CCER &= ~(1 << 1);
	//TIM4->CCER |= (1 << 1);

	TIM4->CCER |= (1 << 0);   // CH1 enable
	TIM4->CCER |= (1 << 4);   // CH2 enable
	TIM4->CCER |= (1 << 8);   // CH3 enable
	TIM4->CCER |= (1 << 12);  // CH4 enable

	TIM4->CCER |= (1 << 0);

	TIM4->EGR |= 1 << 1;
	TIM4->CR1 |= (1 << 0); // enable counter
}
void motor_r_stop()
{
	TIM4->CCR1 = 0;
	TIM4->CCR2 = 0;
}
void motor_l_stop()
{
	TIM4->CCR3 = 0;
	TIM4->CCR4 = 0;
}
void motor_stop()
{
	Motor_Left_Dir = 0;
	Motor_Right_Dir = 0;

	motor_r_stop();
	motor_l_stop();
}


static i16 limit_pwm(i16 pwm)
{
    if (pwm > 999)
        pwm = 999;

    if (pwm < -999)
        pwm = -999;

    return pwm;
}
/*
void motor_run(float power_r, float power_l)
{
  power_r = limit_power(power_r);
  power_l = limit_power(power_l);

  if(fabsf(power_r) >= 0)
  {

	  if (power_r > 0)
	  {
		  pwm_r = (uint32_t)power_r;
		  TIM4->CCR1 = pwm_r;
		  TIM4->CCR2 = 0;
	  }
	  else
	  {
		  pwm_r = (uint32_t)(-power_r);
		  TIM4->CCR1 = 0;
		  TIM4->CCR2 = pwm_r;
	  }
  }
  else if(fabsf(power_l) > 0)
  {
      if (power_l > 0)
	  {
		  pwm_l = (uint32_t)power_l;

		  TIM4->CCR3 = pwm_l;
		  TIM4->CCR4 = 0;
	  }
	  else
	  {
		  pwm_l = (uint32_t)(-power_l);

		  TIM4->CCR3 = 0;
		  TIM4->CCR4 = pwm_l;
	  }
  }
  else
	  {
		  TIM4->CCR3 = 0;
		  TIM4->CCR4 = 0;
		  TIM4->CCR1 = 0;
		  TIM4->CCR2 = 0;
	  }
}
*/
void motor_run(i16 left_pwm, i16 right_pwm)
{
    left_pwm = limit_pwm(left_pwm);
    right_pwm = limit_pwm(right_pwm);

    if (left_pwm > 0)
    {
        Motor_Left_Dir = 1;
        TIM4->CCR1 = left_pwm;
        TIM4->CCR2 = 0;
    }
    else if (left_pwm < 0)
    {
        Motor_Left_Dir = -1;
        TIM4->CCR1 = 0;
        TIM4->CCR2 = -left_pwm;
    }
    else
    {
        Motor_Left_Dir = 0;
        TIM4->CCR1 = 0;
        TIM4->CCR2 = 0;
    }

    if (right_pwm > 0)
    {
        Motor_Right_Dir = 1;
        TIM4->CCR3 = right_pwm;
        TIM4->CCR4 = 0;
    }
    else if (right_pwm < 0)
    {
        Motor_Right_Dir = -1;
        TIM4->CCR3 = 0;
        TIM4->CCR4 = -right_pwm;
    }
    else
    {
        Motor_Right_Dir = 0;
        TIM4->CCR3 = 0;
        TIM4->CCR4 = 0;
    }
}

void motor_init(u8 psc, u16 arr)
{
	motor_pwm_init(psc, arr);
	motor_stop();
}


