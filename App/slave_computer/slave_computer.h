#ifndef _SLAVE_COMPUTER_H_
#define _SLAVE_COMPUTER_H_
#include <stm32f4xx.h>

void Show_Pages(void);
void Page_1(void);
void Page_2(void);
void Page_3(void);

void Line_Op(uint8_t n);
void Data_Op(int32_t *data, int32_t max, int32_t min);
void Flag_Op(uint8_t *flag, uint8_t max, uint8_t min);
void Plus_Op(void);
void Page_Op(void);
void Debug_Op(void);

#endif
