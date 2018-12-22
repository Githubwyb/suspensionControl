#include "main_control.h"

double location_n[2] = {0, 0};
double location_exp[2] = {0, 0};
double length_n[2] = {0, 0};
double length_exp[2] = {0, 0};
int32_t encoder_count_exp[2] = {0, 0};
int32_t encoder_dev[2] = {0, 0};
uint8_t move_mode = 0;                  //运动模式
uint8_t pattern_mode = 0;               //画图方式
uint16_t pattern_n = 250;               //画图精度
uint8_t pattern_start = 0;              //画图开始标志位
double Circle_par[3] = {0, 0, 0};       //圆的参数
double Line_par[4] = {0, 0, 0, 0};      //直线的参数
uint16_t n_now = 0;
uint8_t direction_flag = 0;
double direction_erro = 0.3;
extern uint8_t flag_change;             //数据改变
extern int32_t encoder0_count;
extern int32_t encoder1_count;
extern int32_t motor0_pwm_n;           //左侧电机pwm
extern int32_t motor1_pwm_n;           //右侧电机pwm
extern uint8_t Debug;                  //调试模式

void Control_10ms(void)
{
    int32_t pwm[2] = {0, 0};
    //更新当前各项参数
    length_n[0] = Get_Length0_n();
    length_n[1] = Get_Length1_n();
    Get_Location_n(location_n);

    //获得期望参数
    Get_Length_By_Location(location_exp, length_exp);
    encoder_count_exp[0] = Get_Encoder0_By_Length0(length_exp[0]);
    encoder_count_exp[1] = Get_Encoder1_By_Length1(length_exp[1]);
    encoder_dev[0] = encoder_count_exp[0] - encoder0_count;
    encoder_dev[1] = encoder_count_exp[1] - encoder1_count;
    
    Get_PWM_By_ENCODER(encoder_dev, pwm);
    if(!Debug)
    {
        MOTOR0_Set(pwm[0]);
        motor0_pwm_n = pwm[0];
        MOTOR1_Set(pwm[1]);
        motor1_pwm_n = pwm[1];
    }
    LED_Change(LED_0);
}

void Control_main(void)
{
    static uint8_t cube_flag = 0;
    if(flag_change)
    {
        if(!pattern_mode)
            n_now = 0;
        else
            n_now = pattern_n;
        flag_change = 0;
        cube_flag = 0;
    }
    if(pattern_start)
    {
        switch(move_mode)
        {
            case 0:
                direction_flag = 0;
                break;
            case 1:
                Circle_Location(Circle_par[0], Circle_par[1], Circle_par[2], n_now, location_exp);
                if(Is_match())
                {
                    if(!pattern_mode)
                    {
                        n_now++;
                        if(n_now > pattern_n)
                            n_now = 0;
                    }
                    else
                    {
                        n_now--;
                        if(n_now > pattern_n)
                            n_now = pattern_n;
                    }
                }
                direction_flag = 0;
                break;
            case 2:
                Line_Location(Line_par[0], Line_par[1], Line_par[2], Line_par[3], n_now, location_exp);
                if(Is_match())
                {
                    if(!cube_flag)
                    {
                        n_now++;
                        if(n_now == pattern_n)
                            cube_flag = 1;
                    }
                    else
                    {
                        n_now--;
                        if(n_now == 0)
                            cube_flag = 0;
                    }
                }
                direction_flag = 0;
                break;
            case 3:
                switch(cube_flag)
                {
                    case 0:
                        Line_Location(20, 20, 60, 20, n_now, location_exp);
                        break;
                    case 1:
                        Line_Location(60, 20, 60, 60, n_now, location_exp);
                        break;
                    case 2:
                        Line_Location(60, 60, 20, 60, n_now, location_exp);
                        break;
                    case 3:
                        Line_Location(20, 60, 20, 20, n_now, location_exp);
                        break;
                    default:
                        cube_flag = 0;
                        break;
                }
                if(Is_match())
                {
                    n_now++;
                    if(n_now == pattern_n)
                    {
                        n_now = 0;
                        cube_flag++;
                    }
                }
                direction_flag = 0;
                break;
            case 4:
                if(Is_match())
                {
                    switch(direction_flag)
                    {
                        case 1:
                            location_exp[1] += direction_erro;
                            break;
                        case 2:
                            location_exp[0] += direction_erro;
                            location_exp[1] += direction_erro;
                            break;
                        case 3:
                            location_exp[0] += direction_erro;
                            break;
                        case 4:
                            location_exp[0] += direction_erro;
                            location_exp[1] -= direction_erro;
                            break;
                        case 5:
                            location_exp[1] -= direction_erro;
                            break;
                        case 6:
                            location_exp[0] -= direction_erro;
                            location_exp[1] -= direction_erro;
                            break;
                        case 7:
                            location_exp[0] -= direction_erro;
                            break;
                        case 8:
                            location_exp[0] -= direction_erro;
                            location_exp[1] += direction_erro;
                            break;
                        default:
                            break;
                    }
                }
             //   if(location_exp[0] > 75 
             //       || location_exp[0] < 5
             //       || location_exp[1] > 95
              //      || location_exp[1] < 5)
              //      direction_flag = 0;
                break;
            default:
                break;
        }
    }
}

uint8_t Is_match(void)
{
    if((location_n[0] - location_exp[0]) < 0.5
        && (location_n[0] - location_exp[0]) > -0.5
        && (location_n[1] - location_exp[1]) < 0.5
        && (location_n[1] - location_exp[1]) > -0.5)
        return 1;
    else
        return 0;
}
