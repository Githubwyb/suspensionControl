#ifndef _HANDLE_H_
#define _HANDLE_H_
#include <stm32f4xx.h>

double Get_Length0_n(void);
double Get_Length1_n(void);
int32_t Get_Encoder0_By_Length0(double length);
int32_t Get_Encoder1_By_Length1(double length);
void Get_Location_n(double* location);
void Get_Length_By_Location(double* location, double* length);

#endif // !_LED_H_
