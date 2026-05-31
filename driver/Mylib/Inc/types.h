#ifndef TYPEDS_H
#define TYPEDS_H

typedef float f32;

#define WHEEL_DIAMETER_M    0.065f          /* đường kính bánh */
#define WHEEL_RADIUS_M      (WHEEL_DIAMETER_M * 0.5f)
#define WHEEL_BASE_M        0.170f          /* khoảng cách 2 bánh */
#define ENCODER_PPR         20              /* xung/vòng (1 kênh) */
#define PI                3.14159265f


#define dt_s                0.7f           
#define dt_ms               700u
                      

#define KP_r 0.5f
#define KI_r 0.499f
#define KD_r 0.00015f
#define KP_l 0.5f
#define KI_l 0.499f
#define KD_l 0.00015f
#define pid_int_min       -999.0f
#define pid_int_max          999.0f
#define pid_out_min        -999.0f
#define pid_out_max        999.0f


#define DEG2RAD(x)          ((x) * PI / 180.0f)
#define RAD2DEG(x)          ((x) * 180.0f / PI)
#define limit(x,lo,hi)      ((x)<(lo)?(lo):((x)>(hi)?(hi):(x)))
#define ABS_F(x)            ((x)<0.0f?-(x):(x))





#endif /* CONFIG_H */