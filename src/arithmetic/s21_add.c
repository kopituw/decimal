#include "../s21_decimal.h"

void init_decimal(s21_decimal *decimal)
{
    for (int i = 0; i < 128; i++)
        set_bit(decimal, i, 0);
}

// void norm_decimal(s21_decimal dec1, s21_decimal dec2)
// {
//     int scale1 = get_scale(&dec1), scale2 = get_scale(&dec2);

//     int min_scale = scale1 > scale2 ? scale2 : scale1, max_scale = scale1 > scale2 ? scale2 : scale1;
//     s21_decimal min_dec = min_scale == scale1 ? dec1 : dec2, max_dec = max_scale == scale1 ? dec1 : dec2;
//     s21_decimal ten = {{0, 0, 0, 10}};

//     while (min_scale != max_scale)
//     {
//         if (min_scale < 28 && !s21_mul(min_dec, ten, &min_dec))
//         {
//             min_scale++;
//             set_scale(&min_dec, min_scale);
//         }
//     }

//     set_scale(&min_dec, min_scale);
//     set_scale(&max_dec, min_scale);
// }

int s21_add(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result)
{
    int overflow = normalize(dec_1, dec_2),
        //  int overflow = 0,
        exp = get_scale(&dec_1), sign_1 = get_sign(&dec_1), sign_2 = get_sign(&dec_2);

    // if ((overflow = normalize(dec_1, dec_2)) != OK)
    //     overflow = ;

    if (sign_1 ^ sign_2 && !overflow)
    {
        if (s21_is_greater_or_equal_modal(dec_1, dec_2))
        {
            printf("s21_is_greater_or_equal_modal\n");
            denya_sub_basic(dec_1, dec_2, result);

            if (result->bit[0])
                set_sign(result, sign_1);
        }
        else
        {
            printf("is less\n");
            denya_sub_basic(dec_2, dec_1, result);
            if (result->bit[0])
                set_sign(result, sign_2);
        }
    }
    else if (overflow)
    {
        printf("overflow: %d\n", overflow);
    }
    else
    {

        printf("+\n");
        denya_add_basic(dec_1, dec_2, result);
        if (result->bit[0])
            set_sign(result, sign_1);
    }
    return 1;
}

int denya_add_basic(s21_decimal dec1, s21_decimal dec2, s21_decimal *result)
{
    init_decimal(result);
    int mem = 0;
    for (int i = 0; i < 96; i++)
    {
        int dec1_bit = get_bit(dec1, i);
        int dec2_bit = get_bit(dec2, i);
        set_bit(result, i, dec1_bit ^ dec2_bit ^ mem);
        mem = (dec1_bit && dec2_bit) || (dec1_bit && mem) || (dec2_bit && mem);
    }
    return mem;
}