
#include "control.h"
#include "pid.h"

#include "kinematics.h"
#include "motor.h"
#include "encoder.h"
#include "math.h"



void pid_setup()
{
    PID_Init(&pid_l,KP,KI,KD,pid_out_min,pid_out_max,pid_int_min,pid_int_max);
    PID_Init(&pid_r,KP,KI,KD,pid_out_min,pid_out_max,pid_int_min,pid_int_max);
    
}

void motorcontrol_pid(f32 v, f32 w)
{
    f32 sp_l, sp_r;
    Kinematics_inverse(v,w,&sp_l,&sp_r);

    float out_l = PID_Update(&pid_l, sp_l, encoder_data.velocity_left_mps, dt_s);
    float out_r = PID_Update(&pid_r, sp_r, encoder_data.velocity_right_mps, dt_s);
    
    i16 pwm_l = (i16)limit(out_l,pid_out_min, pid_out_max);
    i16 pwm_r = (i16)limit(out_r, pid_out_min, pid_out_max);
    motor_run(pwm_l,pwm_r);
}