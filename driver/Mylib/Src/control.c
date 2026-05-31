
#include "control.h"
#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "math.h"

Pose_t pose = {0};

void pid_setup()
{
    PID_Init(&pid_l,KP_l,KI_l,KD_l,pid_out_min,pid_out_max,pid_int_min,pid_int_max);
    PID_Init(&pid_r,KP_r,KI_r,KD_r,pid_out_min,pid_out_max,pid_int_min,pid_int_max);
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
void Kinematics_update_pose(float ds_left, float ds_right)
{
    float ds = (ds_right + ds_left) * 0.5f;
    float dth = (ds_right - ds_left) / WHEEL_BASE_M;

    float th_mid = pose.theta + dth * 0.5f;

    pose.x += ds * cosf(th_mid);
    pose.y += ds * sinf(th_mid);
    pose.theta += dth;
    while(pose.theta > PI) pose.theta -= 2.0f * PI;
    while(pose.theta < -PI) pose.theta += 2.0f * PI;

    pose.v = ds / dt_s;
    pose.w = dth / dt_s;
}
void Kinematics_inverse(float v, float w, float *vL_out, float *vR_out)
{

    *vL_out = v - w * (WHEEL_BASE_M * 0.5f);
    *vR_out = v + w * (WHEEL_BASE_M * 0.5f);
    #define VMAX_MPS    0.35f
    float max_abs = ABS_F(*vL_out) > ABS_F(*vR_out)
                  ? ABS_F(*vL_out) : ABS_F(*vR_out);
    if (max_abs > VMAX_MPS) {
        float scale = VMAX_MPS / max_abs;
        *vL_out *= scale;
        *vR_out *= scale;
    }

}

void Kinematics_obs_pos(float dist_m, float servo_deg,  float *obs_x, float *obs_y)
{
    float alpha = pose.theta + DEG2RAD(servo_deg - 90.0f);
    *obs_x = pose.x + dist_m * cosf(alpha);
    *obs_y = pose.y + dist_m * sinf(alpha);
}


void Kinematics_reset(void)
{
    Pose_t Zero = {0};
    pose = Zero;
}