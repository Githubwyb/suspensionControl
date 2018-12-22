#include "pid.h"

double MOTOR_P = 1;             //电机调节P
double MOTOR_D = 0;             //电机调节D
double MOTOR_I = 0;             //电机调节I
uint8_t pid_reset_flag = 0;     //pid静态变量清零标志位
extern uint32_t motor_pwm_max;  //电机最大pwm值

void Get_PWM_By_ENCODER(int32_t* encoder_dev_n, int32_t* pwm)
{
    static int32_t encoder_dev_0[2] = {0, 0};
    static int32_t encoder_dev_sum[2] = {0, 0};
    int32_t max = motor_pwm_max;

    if(pid_reset_flag)
    {
        encoder_dev_sum[0] = 0;
        encoder_dev_sum[1] = 0;
        encoder_dev_0[0] = 0;
        encoder_dev_0[1] = 0;
        pid_reset_flag = 0;
    }
    
    encoder_dev_sum[0] += encoder_dev_n[0];
    encoder_dev_sum[1] += encoder_dev_n[1];
    
    pwm[0] = MOTOR_P * encoder_dev_n[0] + MOTOR_D * (encoder_dev_n[0] - encoder_dev_0[0]) + MOTOR_I * encoder_dev_sum[0];
    pwm[1] = MOTOR_P * encoder_dev_n[1] + MOTOR_D * (encoder_dev_n[1] - encoder_dev_0[1]) + MOTOR_I * encoder_dev_sum[1];
    
    encoder_dev_0[0] = encoder_dev_n[0];
    encoder_dev_0[1] = encoder_dev_n[1];

    if(pwm[0] > max)
        pwm[0] = max;
    else if(pwm[0] < -max)
        pwm[0] = -max;

    if(pwm[1] > max)
        pwm[1] = max;
    else if(pwm[1] < -max)
        pwm[1] = -max;
}
