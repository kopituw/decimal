#include "../s21_decimal.h"

int s21_sub(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result)
{
    int overflow = 0;
    set_sign(&dec_2, !get_sign(&dec_2));
    overflow = s21_add(dec_1, dec_2, result);
    return overflow;
}

void denya_sub_basic(s21_decimal dec1, s21_decimal dec2, s21_decimal *result)
{

    init_decimal(result);
    for (int i = 0; i < 96; i++)
    {
        int dec1_bit = get_bit(dec1, i);
        int dec2_bit = get_bit(dec2, i);
        set_bit(result, i, dec1_bit ^ dec2_bit);

        if (!dec1_bit && dec2_bit)
        {
            int j = i + 1;
            int dec1_bit_new = 0;
            while (!(dec1_bit_new = get_bit(dec1, j)))
            {

                set_bit(&dec1, j, 1);
                j++;
            }

            set_bit(&dec1, j, 0);
        }
    }
}