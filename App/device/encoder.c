#include "encoder.h"
#include "motor.h"

double rotate_motor0 = 0;
double rotate_motor1 = 0;
int32_t encoder0_count = 0;
int32_t encoder1_count = 0;

void ENCODER_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);     //TIM5时钟使能 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);     //TIM2时钟使能   
z
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);   //使能PORTA时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //GPIOA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
    GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA3

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIOB10
    GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PB10

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2); //PB10复用位定时器2
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM5); //PA3复用位定时器5
      
    TIM_TimeBaseStructure.TIM_Prescaler = 83;  //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period = 0xfffff;   //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
    
    //初始化TIM5输入捕获参数
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; 
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;         //上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;     //映射到TI上
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;               //配置输入分频,不分频 
    TIM_ICInitStructure.TIM_ICFilter = 0x00;                            //IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

    //初始化TIM2输入捕获参数
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3; 
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
        
    TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC4,ENABLE);                 //允许更新中断 ,允许CC1IE捕获中断
    TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC3,ENABLE);                 //允许更新中断 ,允许CC1IE捕获中断   
    
    TIM_Cmd(TIM5,ENABLE);                                               //使能定时器5
    TIM_Cmd(TIM2,ENABLE);                                               //使能定时器5
 
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;            //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                  //子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                                     //根据指定的参数初始化NVIC寄存器
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_Init(&NVIC_InitStructure);                                     //根据指定的参数初始化NVIC寄存器
}

void TIM5_IRQHandler(void){
    uint32_t TIME_ENCODER = 0;
    extern int32_t motor0_pwm_n;
    if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){       
        rotate_motor0 = 0;
    }
    else if(TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET){
        TIME_ENCODER = TIM_GetCapture4(TIM5);
        TIM_Cmd(TIM5,DISABLE); //关闭定时器5
        TIM_SetCounter(TIM5,0); //清空计数器
        TIM_Cmd(TIM5,ENABLE);   //使能定时器5
        if(motor0_pwm_n >= 0)
        {
            rotate_motor0 = 1000000.0 / TIME_ENCODER / ENCODER_COUNT;
            encoder0_count++;
        }
        else
        {
            rotate_motor0 = -1000000.0 / TIME_ENCODER / ENCODER_COUNT;
            encoder0_count--;
        }
    }
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC4|TIM_IT_Update); //清除中断标志位
}

void TIM2_IRQHandler(void){
    uint32_t TIME_ENCODER = 0;
    extern int32_t motor1_pwm_n;
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){       
        rotate_motor1 = 0;
    }
    else if(TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET){
        TIME_ENCODER = TIM_GetCapture3(TIM2);
        TIM_Cmd(TIM2,DISABLE);  //关闭定时器2
        TIM_SetCounter(TIM2,0); //清空计数器
        TIM_Cmd(TIM2,ENABLE);   //使能定时器2
        if(motor1_pwm_n >= 0)
        {
            rotate_motor1 = 1000000.0 / TIME_ENCODER / ENCODER_COUNT;
            encoder1_count++;
        }
        else
        {
            rotate_motor1 = -1000000.0 / TIME_ENCODER / ENCODER_COUNT;
            encoder1_count--;
        }
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3|TIM_IT_Update); //清除中断标志位
}
