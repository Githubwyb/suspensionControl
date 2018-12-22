#include<public.h>

int main()
{
    STM32_Init();
    while(1)
    {
        Show_Pages();
        Control_main();
    }
}
