#include "kinematics.h"
#include "types.h"
#include "math.h"

Pose_t pose = {0};
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