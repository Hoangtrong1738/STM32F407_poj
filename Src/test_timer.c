#include "stm32f407xx.h"
#include "delay.h"
#include "clock.h"
#include "uart.h"
//#include "control.h"
#include <stdio.h>
#include "kinematics.h"
#include "encoder.h"
#include "motor.h"
#include "control.h"
#include "types.h"
#include "pid.h"

#include <stdlib.h>
char uart_buf[200];
PID_t pid_l;
PID_t pid_r;
u32 prev_10ms = 0;
float v_l_set = 0.2f;
float v_r_set = 0.2f;
i16 p_l = 0;
i16 p_r = 0;

/*
void Debug_Print(void)
{
    sprintf(uart_buf,
            "PL:%ld PR:%ld "
            "VL:%.2f VR:%.2f "
            "DL:%.2f DR:%.2f\r\n",

            encoder_data.pulse_left,
            encoder_data.pulse_right,

            encoder_data.velocity_left_mps,
            encoder_data.velocity_right_mps,
 
			encoder_data.distance_left_m,
            encoder_data.distance_right_m);

   UART1_Send_String(uart_buf);
}
*/
void Debug_Print(void)
{
    sprintf(uart_buf,
           ">setL:%.3f,velL:%.3f,pwmL:%d,setR:%.3f,velR:%.3f,pwmR:%d\r\n",

            v_l_set,
            encoder_data.velocity_left_mps,
            p_l,
            v_r_set,
            encoder_data.velocity_right_mps,
            p_r

        );

    UART1_Send_String(uart_buf);
}
int main()
{
	clock_init();
	delay_init();
	UART1_init();
	motor_init(84,1000);
	Encoder_init();

	Kinematics_reset();
	pid_setup();
	
	//char buf[80];
	while(1)
	{
		
        
		if(millis() - prev_10ms >= 100)
        {
            prev_10ms = millis();

            EncoderCounter_Update_10ms();

            float o_l = PID_Update(&pid_l,v_l_set,encoder_data.velocity_left_mps,dt_ms);
            float o_r = PID_Update(&pid_r,v_r_set,encoder_data.velocity_right_mps,dt_ms);

            p_l = (i16)limit(o_l,pid_out_min,pid_out_max);
            p_r = (i16)limit(o_r,pid_out_min,pid_out_max);

            motor_run(p_l,p_r);


            Debug_Print();
        }
        
		
	}
	return 0;
}




