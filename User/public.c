#include <public.h>

uint32_t motor_pwm_max = 4000;

void STM32_Init(void)
{
    LED_Init();
    LCD_Init();
    USART1_Init(115200,ENABLE);
    KEY_Init();
    MOTOR_Init(1, motor_pwm_max);
    ENCODER_Init();
    TIM_10ms_Init();
}
