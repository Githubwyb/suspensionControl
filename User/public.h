#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#include <stm32f4xx.h>

#define ENABLE 1
#define DISABLE 0

//driver
#include "usart.h"
#include "delay.h"
#include "tim.h"

//device
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "encoder.h"
#include "motor.h"

//slave_computer
#include "slave_computer.h"

//algorithm
#include "handle.h"
#include "pid.h"
#include "pattern.h"

//control
#include "main_control.h"

void STM32_Init(void);

#endif // _PUBLIC_H
