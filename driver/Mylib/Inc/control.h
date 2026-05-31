#include "types.h"
#include "stm32f407xx.h"

typedef struct {
    float x;
    float y;
    float theta; /* rad*/
    float v; 
    float w; /*rad/s, vận tốc góc*/
}Pose_t;

extern Pose_t pose;
void pid_setup();
void motorcontrol_pid(f32 v, f32 w);


void Kinematics_update_pose(float ds_left, float ds_right);
void Kinematics_inverse(float v, float w, float *vL_out, float *vR_out);

void Kinematics_obs_pos(float dist_m, float servo_deg,  float *obs_x, float *obs_y);

void Kinematics_reset(void);
