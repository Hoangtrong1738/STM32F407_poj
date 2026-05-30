#ifndef TYPEDS_H
#define TYPEDS_H

typedef float f32;

#define WHEEL_DIAMETER_M    0.065f          /* đường kính bánh */
#define WHEEL_RADIUS_M      (WHEEL_DIAMETER_M * 0.5f)
#define WHEEL_BASE_M        0.170f          /* khoảng cách 2 bánh */
#define ENCODER_PPR         20              /* xung/vòng (1 kênh) */
#define PI                3.14159265f


#define dt_s                0.1f           /* 10ms control loop */
#define dt_ms               100u
                      

#define KP 2.5f
#define KI 1.5f
#define KD 0.001f
#define pid_int_min       -700.0f
#define pid_int_max          700.0f
#define pid_out_min        -999.0f
#define pid_out_max        999.0f


#define DEG2RAD(x)          ((x) * PI / 180.0f)
#define RAD2DEG(x)          ((x) * 180.0f / PI)
#define limit(x,lo,hi)      ((x)<(lo)?(lo):((x)>(hi)?(hi):(x)))
#define ABS_F(x)            ((x)<0.0f?-(x):(x))





#endif /* CONFIG_H */