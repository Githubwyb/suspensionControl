#ifndef _PATTERN_H_
#define _PATTERN_H_
#include <stm32f4xx.h>

void Circle_Location(double x, double y, double r, uint16_t n, double* location);
void Line_Location(double x0, double y0, double x1, double y1, uint16_t n, double* location);

#endif // !_PATTERN_H_
