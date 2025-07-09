#include "../s21_decimal.h"

int s21_big_is_equal(s21_big_decimal value1, s21_big_decimal value2)
{
    int flag = 1;
    for (int i = 255; i >= 0 && flag; i--)
    {
        int bit1 = big_get_bit(value1, i);
        int bit2 = big_get_bit(value2, i);
        if (bit1 != bit2)
            flag = 0;
    }
    return flag;
}

int s21_is_equal(s21_decimal dec1, s21_decimal dec2)
{
    return dec1.bit[2] == dec2.bit[2] && dec1.bit[1] == dec2.bit[1] && dec1.bit[0] == dec2.bit[0];
}
