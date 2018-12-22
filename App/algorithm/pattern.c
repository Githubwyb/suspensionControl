#include "pattern.h"
#include "math.h"

#define PI 3.1415926535
extern uint16_t pattern_n;               //画图精度

void Circle_Location(double x, double y, double r, uint16_t n, double* location)
{
    double angle = 0;
    angle = -PI + n * 2 * PI / pattern_n;
    location[0] = r * cos(angle) + x;
    location[1] = r * sin(angle) + y;
}

void Line_Location(double x0, double y0, double x1, double y1, uint16_t n, double* location)
{
    location[0] = n * (x1 - x0) / pattern_n + x0;
    location[1] = n * (y1 - y0) / pattern_n + y0;
}
