#include "../s21_decimal.h"

int s21_big_is_greater(s21_big_decimal value1, s21_big_decimal value2)
{
    int result = 0;
    int is_comparing = 1;
    for (int i = 255; i >= 0 && is_comparing; i--)
    {
        int bit1 = big_get_bit(value1, i);
        int bit2 = big_get_bit(value2, i);
        if (bit1 > bit2)
        {
            result = 1;
            is_comparing = 0;
        }
        if (bit1 < bit2)
        {
            result = 0;
            is_comparing = 0;
        }
    }
    return result;
}

int s21_is_greater(s21_decimal dec1, s21_decimal dec2)
{
    int result = -1;
    for (int i = 2; i >= 0 && result == -1; i--)
    {
        if (dec1.bit[i] != dec2.bit[i])
            result = dec1.bit[i] > dec2.bit[i];
    }
    if (result == -1)
        result = 0;

    return result;
}