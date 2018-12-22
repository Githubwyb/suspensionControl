#include "handle.h"
#include "math.h"

#define LENGTH_ENCODER_0 2679.26
#define LENGTH_ENCODER_1 2663.01

extern int32_t encoder0_count;
extern int32_t encoder1_count;
extern double length_n[2];

double Get_Length0_n(void)
{
    return encoder0_count / LENGTH_ENCODER_0;
}

double Get_Length1_n(void)
{
    return encoder1_count / LENGTH_ENCODER_1;
}

int32_t Get_Encoder0_By_Length0(double length)
{
    return length * LENGTH_ENCODER_0;
}

int32_t Get_Encoder1_By_Length1(double length)
{
    return length * LENGTH_ENCODER_1;
}

void Get_Location_n(double* location)
{
    double S, l, h;
    l = 115.974135 - length_n[0] + 149.164339 - length_n[1] + 110;
    S = sqrt(l / 2 * (l / 2 - 115.974135 + length_n[0]) * (l / 2 - 149.164339 + length_n[1]) * (l / 2 - 110));
    h = 2 * S / 110;
    location[0] = sqrt(pow(115.974135 - length_n[0], 2) - pow(h, 2)) - 15;
    location[1] = 115 - h;
}

void Get_Length_By_Location(double* location, double* length)
{
    length[0] = 115.974135 - sqrt(pow(location[0] + 15, 2) + pow(115 - location[1], 2));
    length[1] = 149.164339 - sqrt(pow(95 - location[0], 2) + pow(115 - location[1], 2));
}
