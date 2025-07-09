#include "../s21_decimal.h"

int last_bit(s21_decimal numb)
{
    int last_bit = 95;
    while (last_bit >= 0 && get_bit(numb, last_bit) == 0)
    {
        last_bit--;
    }
    return last_bit;
}

int big_last_bit(s21_big_decimal numb)
{
    int last_bit = 255;
    while (last_bit >= 0 && big_get_bit(numb, last_bit) == 0)
    {
        last_bit--;
    }
    return last_bit;
}
