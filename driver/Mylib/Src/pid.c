/*
 * pid.c
 *
 *  Created on: May 20, 2026
 *      Author: GB Center
 */
#include "pid.h"


static float PID_Clamp(float value, float min_value, float max_value)
{
	if(value > max_value) return max_value;
	if(value < min_value ) return min_value;
	return value;
}

void PID_Init(PID_t *pid, float kp ,float ki, float kd, float output_min,
				float output_max, float integral_min, float intrgral_max)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;

	pid->error = 0.0f;
	pid->prev_error = 0.0f;
	pid->integral = 0.0f;
	pid->output = 0.0f;

	pid->output_min = output_min;
	pid->output_max = output_max;
	pid->integral_min = integral_min;
	pid->integral_max = intrgral_max;

}

void PID_Reset(PID_t *pid)
{

	pid->error = 0.0f;
	pid->prev_error = 0.0f;
	pid->integral = 0.0f;
	pid->output = 0.0f;

}

float PID_Update(PID_t *pid, float target, float feedback, float dt)
{

	if(pid == 0) return 0.0f;
	if(dt <= 0.0f) return pid->output;

	pid->error = target - feedback;

	pid->integral += pid->error * dt;
	pid->integral = PID_Clamp(pid->integral, pid->integral_min, pid->integral_max);

	float derivative = (pid->error - pid->prev_error) / dt;
	pid->output = pid->kp * pid->error + pid->ki * pid->integral + pid->kd * derivative;

	pid->prev_error = pid->error;

	return pid->output;
	
	/*
	   if (!pid) return 0.0f;
    if (dt <= 0.0f) return pid->output;

    // 1. error
    float error = target - feedback;

    // 2. derivative (filtered nhẹ)
    float derivative = (error - pid->prev_error) / dt;

    // optional low-pass D (rất nên có)
    derivative = 0.7f * derivative;

    // 3. tentative output (chưa integral)
    float p = pid->kp * error;
    float i = pid->ki * pid->integral;
    float d = pid->kd * derivative;

    float output = p + i + d;

    // 4. ANTI-WINDUP LOGIC
    int saturated_high = (output > pid->output_max);
    int saturated_low  = (output < pid->output_min);

    // chỉ tích integral khi KHÔNG bị saturate
    if (!((saturated_high && error > 0) ||
          (saturated_low  && error < 0)))
    {
        pid->integral += error * dt;

        // clamp integral
        if (pid->integral > pid->integral_max)
            pid->integral = pid->integral_max;
        else if (pid->integral < pid->integral_min)
            pid->integral = pid->integral_min;
    }

    // 5. recompute output sau anti-windup
    output = pid->kp * error +
             pid->ki * pid->integral +
             pid->kd * derivative;

    // 6. clamp final output
    if (output > pid->output_max) output = pid->output_max;
    if (output < pid->output_min) output = pid->output_min;

    // 7. save state
    pid->prev_error = error;
    pid->output = output;

    return output;
	*/
}
