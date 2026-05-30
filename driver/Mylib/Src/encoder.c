/*
 * encoder.c
 *
 *  Created on: May 14, 2026
 *      Author: GB Center
 */

#include "encoder.h"
#include "motor.h"
#include "kinematics.h"

Encoder_data_t encoder_data = {0};
static const float distance_per_pulse = (PI * WHEEL_DIAMETER_M) / ENCODER_PPR;


static void Encoder_L_init()//TIMER 3
{
	GPIO_Handle_t C;
	C.pGPIOx = GPIOC;
	C.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFM;
	C.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	C.GPIO_PinConfig.GPIO_PinAltFunMode = 2; //AF2

	C.GPIO_PinConfig.GPIO_PinNumber = PIN_7;
	GPIO_Init(&C);

	TIM3_PCLK_EN();

	TIM3->ARR = 0xffffffff;
	TIM3->PSC = 0;
	TIM3->CNT = 0;

	TIM3->CCMR1 &= ~(0b11 << 8);
	TIM3->CCMR1 |= (0b01 << 8);

	TIM3->CCMR1 &= ~(0b111 << 12);
	TIM3->CCMR1 |= (0b111 << 12);

	TIM3->CCER &= ~(1 << 4);
	TIM3->CCER |= (1 << 4); // enable channel input capture

	TIM3 ->SMCR &= ~(0b111 << 4);
	TIM3 ->SMCR |= (0b110 << 4);
	TIM3 ->SMCR &= ~(0b111 << 0);
	TIM3 ->SMCR |= (0b111 << 0);

	TIM3->CR1 |= 1 << 0;


}
static void Encoder_R_init()//TIMER 5
{
	GPIO_Handle_t A;
	A.pGPIOx = GPIOA;
	A.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFM;
	A.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	A.GPIO_PinConfig.GPIO_PinAltFunMode = 2;

	A.GPIO_PinConfig.GPIO_PinNumber = PIN_1;
	GPIO_Init(&A);

	TIM5_PCLK_EN();
	TIM5->ARR = 0xffffffff;
	TIM5->PSC = 0;
	TIM5->CNT = 0;

	TIM5->CCMR1 &= ~(0b11 << 8);
	TIM5->CCMR1 |= (0b01 << 8);

	TIM5->CCMR1 &= ~(0b111 << 12);
	TIM5->CCMR1 |= (0b111 << 12);

	TIM5->CCER &= ~(1 << 4);
	TIM5->CCER |= (1 << 4); // enable channel 2 input capture
	//TIM5->CCMR1 &= ~(0b111 << 0);
	//TIM5->CCMR1 |= (0b111 << 0); //External Clock mode 1 - Rising edges of the selected trigger (TRGI) clock the counter

	TIM5->SMCR &= ~(0b111 << 4);
	TIM5->SMCR |= (0b110 << 4);
	TIM5->SMCR &= ~(0b111 << 0);
	TIM5->SMCR |= (0b111 << 0);


	TIM5->CR1 |= 1<<0;

}
void EncoderCounter_Update_10ms()
{
	i32 left = (i32)TIM3->CNT;
	i32 right = (i32)TIM5->CNT;

	float ds_left;
	float ds_right;
	float v_left;
	float v_right;

	TIM3->CNT = 0;
	TIM5->CNT = 0;

	if(Motor_Left_Dir < 0) left = -left;
	if(Motor_Right_Dir < 0) right = -right;

	encoder_data.pulse_left = left;
	encoder_data.pulse_right = right;

	encoder_data.total_pulse_left +=left;
	encoder_data.total_pulse_right +=right;

	ds_left = (float)left * distance_per_pulse;
	ds_right = (float)right * distance_per_pulse;

	encoder_data.distance_robot_m = (encoder_data.distance_left_m + encoder_data.distance_right_m) * 0.5;
	v_left = ds_left / dt_s;
	v_right = ds_right / dt_s;
	encoder_data.velocity_left_mps = v_left;
	encoder_data.velocity_right_mps = v_right;
	encoder_data.velocity_robot_mps = (encoder_data.velocity_left_mps + encoder_data.velocity_right_mps) * 0.5;

	Kinematics_update_pose(ds_left,ds_right);
}
void Encoder_init()
{
	Encoder_L_init();
	Encoder_R_init();
	Encoder_reset();
}

void Encoder_reset()
{
	TIM3->CNT = 0;
	TIM5->CNT = 0;

	/*Zero toàn bộ struct*/
	Encoder_data_t Zero = {0};

	encoder_data = Zero;
}













