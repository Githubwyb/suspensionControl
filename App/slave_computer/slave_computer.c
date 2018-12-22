#include "slave_computer.h"
#include "motor.h"
#include "key.h"
#include "encoder.h"
#include "lcd.h"
#include "handle.h"

#define DATA_LOCATION 160

uint8_t key = 4;                    //当前键值
int32_t motor0_pwm_n = 0;           //左侧电机pwm
int32_t motor1_pwm_n = 0;           //右侧电机pwm
uint8_t Debug = 1;                  //调试模式
uint8_t page_max = 3;               //最大页数
uint32_t plus = 1;                  //增量单位
uint8_t line_n = 0;                 //当前行数
uint8_t Page = 1;                   //当前页码
uint8_t flag_change = 0;            //参数改变
double location_diy[2] = {0,0};
extern double rotate_motor0;        //左侧电机转速
extern double rotate_motor1;        //右侧电机转速
extern int32_t encoder0_count;      //左侧编码器总码数
extern int32_t encoder1_count;      //右侧编码器总码数
extern uint32_t motor_pwm_max;      //电机最大pwm值
extern uint8_t pid_reset_flag;      //pid静态变量清零标志位
extern double MOTOR_P;              //电机调节P
extern double MOTOR_D;              //电机调节D
extern double MOTOR_I;              //电机调节I
extern double location_n[2];        //当前位置
extern double location_exp[2];      //期望位置
extern double length_n[2];          //当前线长
extern double length_exp[2];        //期望线长
extern int32_t encoder_count_exp[2];//期望编码器总码数
extern int32_t encoder_dev[2];      //编码器码数差值
extern uint8_t move_mode;           //运动模式
extern uint8_t pattern_mode;        //画图方式
extern uint16_t pattern_n;          //画图精度
extern uint8_t pattern_start;       //画图开始标志位
extern double Circle_par[3];        //圆的参数
extern double Line_par[4];          //直线的参数
extern uint16_t n_now;
extern uint8_t direction_flag;

void Show_Pages(void){
	key = KEY_S(0);
	switch(line_n){
		case 0:
			Page_Op();
			break;
		case 1:
			Debug_Op();
			break;
	}
	switch(Page){
		case 1:
			Page_1();
			break;
		case 2:
			Page_2();
			break;
        case 3:
            Page_3();
            break;
	}
}

void Page_1(void){
	uint16_t line[18] = {10,30,70,90,110,130,150,170,210,230,250,270,290,310,330,350,370,390};
	int32_t data[4];
	LCD_ShowChar(5, line[line_n], '>', 16, 1);
	LCD_ShowString(20, line[0], 200, 16, 16, "Page:");
	LCD_ShowNum(DATA_LOCATION, line[0], Page, 3, 16);
	LCD_ShowString(20, line[1], 200, 16, 16, "Debug:");
	LCD_ShowNum(DATA_LOCATION, line[1], Debug, 3, 16);

	LCD_ShowString(20, line[2], 200, 16, 16, "Plus:");
	LCD_ShowNum(DATA_LOCATION, line[2], plus, 10, 16);
	LCD_ShowString(20, line[3], 200, 16, 16, "MOTOR_P:");
	LCD_ShowNum(DATA_LOCATION, line[3], (int32_t)(MOTOR_P * 1000), 10, 16);
	LCD_ShowString(20, line[4], 200, 16, 16, "MOTOR_D:");
	LCD_ShowNum(DATA_LOCATION, line[4], (int32_t)(MOTOR_D * 1000), 10, 16);
	LCD_ShowString(20, line[5], 200, 16, 16, "MOTOR_I:");
	LCD_ShowNum(DATA_LOCATION, line[5], (int32_t)(MOTOR_I * 1000), 10, 16);
	LCD_ShowString(20, line[6], 200, 16, 16, "locat_exp_x:");
	LCD_ShowNum(DATA_LOCATION, line[6], (int32_t)((location_exp[0] + location_diy[0]) * 10), 10, 16);
    LCD_ShowString(20, line[7], 200, 16, 16, "locat_exp_y:");
    LCD_ShowNum(DATA_LOCATION, line[7], (int32_t)((location_exp[1] + location_diy[1]) * 10), 10, 16);

	LCD_ShowString(20, line[8], 200, 16, 16, "motor0_pwm:");
	LCD_ShowNum(DATA_LOCATION, line[8], motor0_pwm_n, 10, 16);
	LCD_ShowString(20, line[9], 200, 16, 16, "motor1_pwm:");
	LCD_ShowNum(DATA_LOCATION, line[9], motor1_pwm_n , 10, 16);
    LCD_ShowString(20, line[10], 200, 16, 16, "locat_x:");
    LCD_ShowNum(DATA_LOCATION, line[10], (int32_t)((location_n[0] + location_diy[0]) * 10), 10, 16);
    LCD_ShowString(20, line[11], 200, 16, 16, "locat_y:");
    LCD_ShowNum(DATA_LOCATION, line[11], (int32_t)((location_n[1] + location_diy[1]) * 10), 10, 16);
    LCD_ShowString(20, line[12], 200, 16, 16, "length0:");
    LCD_ShowNum(DATA_LOCATION, line[12], (int32_t)(length_n[0] * 1000), 10, 16);
    LCD_ShowString(20, line[13], 200, 16, 16, "length1:");
    LCD_ShowNum(DATA_LOCATION, line[13], (int32_t)(length_n[1] * 1000), 10, 16);
    LCD_ShowString(20, line[14], 200, 16, 16, "encoder0:");
    LCD_ShowNum(DATA_LOCATION, line[14], encoder0_count, 10, 16);
    LCD_ShowString(20, line[15], 200, 16, 16, "encoder1:");
    LCD_ShowNum(DATA_LOCATION, line[15], encoder1_count, 10, 16);
    LCD_ShowString(20, line[16], 200, 16, 16, "encoder0_exp:");
    LCD_ShowNum(DATA_LOCATION, line[16], encoder_count_exp[0], 10, 16);
    LCD_ShowString(20, line[17], 200, 16, 16, "encoder1_exp:");
    LCD_ShowNum(DATA_LOCATION, line[17], encoder_count_exp[1], 10, 16);

	Line_Op(7);
	switch(line_n){
		case 2:
			Plus_Op();
			break;
		case 3:
			data[0] = MOTOR_P * 1000;
			Data_Op(data, 999999999, -999999999);
			MOTOR_P = data[0];
			MOTOR_P /= 1000;
			break;
		case 4:
			data[0] = MOTOR_D * 1000;
			Data_Op(data, 999999999, -999999999);
			MOTOR_D = data[0];
			MOTOR_D /= 1000;
			break;
		case 5:
			data[0] = MOTOR_I * 1000;
			Data_Op(data, 999999999, -999999999);
			MOTOR_I = data[0];
			MOTOR_I /= 1000;
			break;
		case 6:
			data[0] = location_exp[0] * 10;
			Data_Op(data, 1000, -1000);
			location_exp[0] = data[0];
			location_exp[0] /= 10;
			break;
        case 7:
            data[0] = location_exp[1] * 10;
            Data_Op(data, 1000, -1000);
            location_exp[1] = data[0];
            location_exp[1] /= 10;
            break;
	}
}

void Page_2(void){
    uint16_t line[18] = {10,30,70,90,110,150,170,190,210,230,250,270,290,310,350,350,370,390};
    int32_t data[4];
    LCD_ShowChar(5, line[line_n], '>', 16, 1);
    LCD_ShowString(20, line[0], 200, 16, 16, "Page:");
    LCD_ShowNum(DATA_LOCATION, line[0], Page, 3, 16);
    LCD_ShowString(20, line[1], 200, 16, 16, "Debug:");
    LCD_ShowNum(DATA_LOCATION, line[1], Debug, 3, 16);

    LCD_ShowString(20, line[2], 200, 16, 16, "move_mode:");
    LCD_ShowNum(DATA_LOCATION, line[2], move_mode, 10, 16);
    LCD_ShowString(20, line[3], 200, 16, 16, "mode:");
    LCD_ShowNum(DATA_LOCATION, line[3], pattern_mode, 10, 16);
    LCD_ShowString(20, line[4], 200, 16, 16, "start:");
    LCD_ShowNum(DATA_LOCATION, line[4], pattern_start, 10, 16);

    LCD_ShowString(20, line[5], 200, 16, 16, "Plus:");
    LCD_ShowNum(DATA_LOCATION, line[5], plus, 10, 16);
    LCD_ShowString(20, line[6], 200, 16, 16, "pattern_n:");
    LCD_ShowNum(DATA_LOCATION, line[6], pattern_n, 10, 16);
    LCD_ShowString(20, line[7], 200, 16, 16, "Circle_x:");
    LCD_ShowNum(DATA_LOCATION, line[7], (int32_t)((Circle_par[0] + location_diy[0]) * 10), 10, 16);
    LCD_ShowString(20, line[8], 200, 16, 16, "Circle_y:");
    LCD_ShowNum(DATA_LOCATION, line[8], (int32_t)((Circle_par[1] + location_diy[1]) * 10), 10, 16);
    LCD_ShowString(20, line[9], 200, 16, 16, "Circle_r:");
    LCD_ShowNum(DATA_LOCATION, line[9], (int32_t)(Circle_par[2] * 10), 10, 16);
    LCD_ShowString(20, line[10], 200, 16, 16, "Line_x0:");
    LCD_ShowNum(DATA_LOCATION, line[10], (int32_t)((Line_par[0] + location_diy[0]) * 10), 10, 16);
    LCD_ShowString(20, line[11], 200, 16, 16, "Line_y0:");
    LCD_ShowNum(DATA_LOCATION, line[11], (int32_t)((Line_par[1] + location_diy[1]) * 10), 10, 16);
    LCD_ShowString(20, line[12], 200, 16, 16, "Line_x1:");
    LCD_ShowNum(DATA_LOCATION, line[12], (int32_t)((Line_par[2] + location_diy[0]) * 10), 10, 16);
    LCD_ShowString(20, line[13], 200, 16, 16, "Line_y1:");
    LCD_ShowNum(DATA_LOCATION, line[13], (int32_t)((Line_par[3] + location_diy[1]) * 10), 10, 16);

    LCD_ShowString(20, line[14], 200, 16, 16, "Direction:");
    LCD_ShowNum(DATA_LOCATION, line[14], direction_flag, 10, 16);

    Line_Op(13);
    switch(line_n){
        case 2:
            Flag_Op(&move_mode, 4, 0);
            break;
        case 3:
            Flag_Op(&pattern_mode, 1, 0);
            flag_change = 0;
            break;
        case 4:
            Flag_Op(&pattern_start, 1, 0);
            flag_change = 0;
            break;
        case 5:
            Plus_Op();
            break;
        case 6:
            data[0] = pattern_n;
            Data_Op(data, 499, 0);
            pattern_n = data[0];
            break;
        case 7:
            data[0] = Circle_par[0] * 10;
            Data_Op(data, 1000, -1000);
            Circle_par[0] = data[0];
            Circle_par[0] /= 10;
            break;
        case 8:
            data[0] = Circle_par[1] * 10;
            Data_Op(data, 1000, -1000);
            Circle_par[1] = data[0];
            Circle_par[1] /= 10;
            break;
        case 9:
            data[0] = Circle_par[2] * 10;
            Data_Op(data, 400, 0);
            Circle_par[2] = data[0];
            Circle_par[2] /= 10;
            break;
        case 10:
            data[0] = Line_par[0] * 10;
            Data_Op(data, 1000, -1000);
            Line_par[0] = data[0];
            Line_par[0] /= 10;
            break;
        case 11:
            data[0] = Line_par[1] * 10;
            Data_Op(data, 1000, -1000);
            Line_par[1] = data[0];
            Line_par[1] /= 10;
            break;
        case 12:
            data[0] = Line_par[2] * 10;
            Data_Op(data, 1000, -1000);
            Line_par[2] = data[0];
            Line_par[2] /= 10;
            break;
        case 13:
            data[0] = Line_par[3] * 10;
            Data_Op(data, 1000, -1000);
            Line_par[3] = data[0];
            Line_par[3] /= 10;
            break;
    }
}

void Page_3(void){
	uint16_t line[11] = {10,30,70,90,110,130,150,190,210,230,250};
	int32_t data[4];
	LCD_ShowChar(5, line[line_n], '>', 16, 1);
	LCD_ShowString(20, line[0], 200, 16, 16, "Page:");
	LCD_ShowNum(DATA_LOCATION, line[0], Page, 3, 16);
	LCD_ShowString(20, line[1], 200, 16, 16, "Debug:");
	LCD_ShowNum(DATA_LOCATION, line[1], Debug, 3, 16);

	LCD_ShowString(20, line[2], 200, 16, 16, "Plus:");
	LCD_ShowNum(DATA_LOCATION, line[2], plus, 10, 16);
	LCD_ShowString(20, line[3], 200, 16, 16, "motor0_pwm:");
	LCD_ShowNum(DATA_LOCATION, line[3], motor0_pwm_n, 10, 16);
    LCD_ShowString(20, line[4], 200, 16, 16, "motor1_pwm:");
    LCD_ShowNum(DATA_LOCATION, line[4], motor1_pwm_n, 10, 16);
    LCD_ShowString(20, line[5], 200, 16, 16, "location_n_x:");
    LCD_ShowNum(DATA_LOCATION, line[5], (int32_t)((location_n[0] + location_diy[0]) * 10), 10, 16);
    LCD_ShowString(20, line[6], 200, 16, 16, "location_n_y:");
    LCD_ShowNum(DATA_LOCATION, line[6], (int32_t)((location_n[1] + location_diy[1]) * 10), 10, 16);
    
	LCD_ShowString(20, line[7], 200, 16, 16, "rot_motor0:");
	LCD_ShowNum(DATA_LOCATION, line[7], (int32_t)(rotate_motor0 * 1000), 10, 16);
    LCD_ShowString(20, line[8], 200, 16, 16, "rot_motor1:");
    LCD_ShowNum(DATA_LOCATION, line[8], (int32_t)(rotate_motor1 * 1000), 10, 16);
    LCD_ShowString(20, line[9], 200, 16, 16, "enc0_count:");
    LCD_ShowNum(DATA_LOCATION, line[9], encoder0_count, 10, 16);
    LCD_ShowString(20, line[10], 200, 16, 16, "enc1_count:");
    LCD_ShowNum(DATA_LOCATION, line[10], encoder1_count, 10, 16);


	if(Debug){
		Line_Op(6);

		switch(line_n){
			case 2:
				Plus_Op();
				break;
			case 3:
				data[0] = motor0_pwm_n;
				Data_Op(data, motor_pwm_max, -motor_pwm_max);
				motor0_pwm_n = data[0];
				MOTOR0_Set(motor0_pwm_n);
				break;
            case 4:
                data[0] = motor1_pwm_n;
                Data_Op(data, motor_pwm_max, -motor_pwm_max);
                motor1_pwm_n = data[0];
                MOTOR1_Set(motor1_pwm_n);
                break;
            case 5:
                data[0] = location_diy[0] * 10;
                Data_Op(data, 1000, -1000);
                location_diy[0] = data[0];
                location_diy[0] /= 10;
                break;
            case 6:
                data[0] = location_diy[1] * 10;
                Data_Op(data, 1000, -1000);
                location_diy[1] = data[0];
                location_diy[1] /= 10;
                break;
		}
	}
	else
		Line_Op(1);
}

void Data_Op(int32_t *data, int32_t max, int32_t min){
	switch(key){
		case 0:
			*data += plus;
            flag_change = 1;
			if(*data > max)
				*data = max;
			break; 
		case 2:
            flag_change = 1;
			*data -= plus;
			if(*data < min)
				*data = min;
			break; 
	}
}

void Flag_Op(uint8_t *flag, uint8_t max, uint8_t min)
{
    switch(key)
    {
        case 0:
            if(*flag != max)
                (*flag)++;
            flag_change = 1;
            break; 
        case 2:
            if(*flag != min)
                (*flag)--;
            flag_change = 1;
            break;
        default:
            break;
    }
}

void Page_Op(void){
	switch(key){
		case 0:
			if(Page != page_max)
				Page++;
			LCD_Clear(WHITE);
			break;
		case 2:
			if(Page != 1)
				Page--;
			LCD_Clear(WHITE);
			break;
	}
}

void Debug_Op(void){
	switch(key){
		case 0:
			if(Debug != 2)
				Debug++;
			MOTOR0_Set(0);
			MOTOR1_Set(0);
            motor0_pwm_n = 0;
            motor1_pwm_n = 0;
            pid_reset_flag = 1;
            if(Debug == 2)
            {
                encoder0_count = 0;
                encoder1_count = 0;
                Debug = 1;
            }
			break;
		case 2:
			if(Debug != 0)
				Debug--;
			MOTOR0_Set(0);
            MOTOR1_Set(0);
            motor0_pwm_n = 0;
            motor1_pwm_n = 0;
            pid_reset_flag = 1;
			break;
	}
}

void Line_Op(uint8_t n){
	switch(key){
		case 3:
			if(line_n != 0){
				line_n--;
				LCD_Clear(WHITE);
			}
			break; 
		case 1:
			if(line_n != n){
				line_n++;
				LCD_Clear(WHITE);
			}
			break; 
	}
}

void Plus_Op(void){
	switch(key){
		case 0:
			switch(plus){
				case 1:
					plus = 10;
					break;
				case 10:
					plus = 100;
					break;
				case 100:
					plus = 1000;
					break;
				case 1000:
					plus = 10000;
					break;
				case 10000:
					plus = 100000;
					break;
				case 100000:
					plus = 1000000;
					break;
				case 1000000:
					plus = 10000000;
					break;
				case 10000000:
					plus = 100000000;
					break;
				case 100000000:
					plus = 1000000000;
					break;
				case 1000000000:
					plus = 1;
					break;
			}
			break;
		case 2:
			switch(plus){
				case 1:
					plus = 1000000000;
					break;
				case 10:
					plus = 1;
					break;
				case 100:
					plus = 10;
					break;
				case 1000:
					plus = 100;
					break;
				case 10000:
					plus = 1000;
					break;
				case 100000:
					plus = 10000;
					break;
				case 1000000:
					plus = 100000;
					break;
				case 10000000:
					plus = 1000000;
					break;
				case 100000000:
					plus = 10000000;
					break;
				case 1000000000:
					plus = 100000000;
					break;
			}
			break;
	}
}
