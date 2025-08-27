
#include "../s21_decimal.h"
#include <math.h>
#include <stdlib.h>

int s21_from_float_to_decimal(float src, s21_decimal *dst)
{
    null_decimal(dst);
    int return_value = 0;
    if (isinf(src) || isnan(src))
    {
        return_value = 1;
    }
    else
    {
        if (fabs(src) < 1e-28)
        {
            null_decimal(dst);
            return_value = 0;
        }
        else if (src != 0)
        {
            int sign = *(int *)&src >> 31;
            int exp = ((*(int *)&src & ~0x80000000) >> 23) - 127;
            double temp = (double)fabs(src);
            int off = 0;

            while (temp < 1e-28 && off < 28)
            {
                temp *= 10;
                off++;
            }
            temp = round(temp);

            if (temp == 0)
            {
                null_decimal(dst);
            }
            else if (off <= 28 && (exp > -94 && exp < 96))
            {
                unsigned int int_part = (unsigned int)temp;

                for (int i = 0; i < 96 && int_part > 0; i++)
                {
                    if (int_part & 1)
                    {
                        dst->bit[i / 32] |= 1 << (i % 32);
                    }
                    int_part >>= 1;
                }

                dst->bit[3] = (sign << 31) | (off << 16);
            }
            else
            {
                return_value = 1;
            }
        }
    }
    return return_value;
}