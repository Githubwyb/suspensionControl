#ifndef _MAIN_CONTROL_H_
#define _MAIN_CONTROL_H_
#include "public.h"

void Control_10ms(void);
void Control_main(void);
void pattern_print(double location[][2], uint8_t mode);
uint8_t Is_match(void);

#endif // !_LED_H_
