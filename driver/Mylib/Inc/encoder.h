/*
 * encoder.h
 *
 *  Created on: May 14, 2026
 *      Author: GB Center
 */

#ifndef MYLIB_INC_ENCODER_H_
#define MYLIB_INC_ENCODER_H_
#include "stm32f407xx.h"
#include "types.h"
typedef struct
{
	i32 pulse_left;
	i32 pulse_right;

	i32 total_pulse_left;
	i32 total_pulse_right;

	float distance_left_m;
	float distance_right_m;
	float distance_robot_m;

	float velocity_left_mps;
	float velocity_right_mps;
	float velocity_robot_mps;
}Encoder_data_t;

extern Encoder_data_t encoder_data;


void Encoder_init();
void Encoder_reset();
void EncoderCounter_Update();
void EncoderCounter_ResetDistance();


#endif /* MYLIB_INC_ENCODER_H_ */
