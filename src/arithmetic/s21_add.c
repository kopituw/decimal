#include "../s21_decimal.h"

void init_decimal(s21_decimal *decimal)
{
    for (int i = 0; i < 128; i++)
        set_bit(decimal, i, 0);
}

void norm_decimal(s21_decimal dec1, s21_decimal dec2)
{
    int scale1 = get_scale(&dec1), scale2 = get_scale(&dec2);

    int min_scale = scale1 > scale2 ? scale2 : scale1, max_scale = scale1 > scale2 ? scale2 : scale1;
    s21_decimal min_dec = min_scale == scale1 ? dec1 : dec2, max_dec = max_scale == scale1 ? dec1 : dec2;
    s21_decimal ten = {{0, 0, 0, 10}};

    while (min_scale != max_scale)
    {
        if (min_scale < 28 && !s21_mul(min_dec, ten, &min_dec))
        {
            min_scale++;
            set_scale(&min_dec, min_scale);
        }
    }

    set_scale(&min_dec, min_scale);
    set_scale(&max_dec, min_scale);
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

    // printf("%u + %u = %u\n", dec1.bit[0], dec2.bit[0], result->bit[0]);
    return mem;
}

void denya_sub_basic(s21_decimal dec1, s21_decimal dec2, s21_decimal *result)
{
    init_decimal(result);
    for (int i = 0; i < 32; i++)
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

int s21_add(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result)
{
    int overflow = 0, exp = get_scale(&dec_1), sign_1 = get_sign(&dec_1), sign_2 = get_sign(&dec_2);
    norm_decimal(dec_1, dec_2);
    if (sign_1 ^ sign_2)
    {
        if (s21_is_greater_or_equal(dec_1, dec_2))
        {
            denya_sub_basic(dec_1, dec_2, result);

            if (result->bit[0])
                set_sign(result, sign_1);
        }
        else
        {
            denya_sub_basic(dec_2, dec_1, result);
            if (result->bit[0])
                set_sign(result, sign_2);
        }
    }
    else
    {
        denya_add_basic(dec_1, dec_2, result);
        if (result->bit[0])
            set_sign(result, sign_1);
    }
    return 1;
}

int s21_sub(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result)
{
    int overflow = 0;
    set_sign(&dec_2, !get_sign(&dec_2));
    s21_add(dec_1, dec_2, result);
    return overflow;
}

int int_to_binary(unsigned int integer, char *hexadecimal)
{
    int hex_counter = 0;

    if (integer == 0)
        hexadecimal[hex_counter++] = '0';

    while (integer)
    {
        int ostatok = 0;

        ostatok = integer % 16;
        integer /= 16;

        if (ostatok == 10)
        {
            hexadecimal[hex_counter++] = 'a';
        }
        else if (ostatok == 11)
        {
            hexadecimal[hex_counter++] = 'b';
        }
        else if (ostatok == 12)
        {
            hexadecimal[hex_counter++] = 'c';
        }
        else if (ostatok == 13)
        {
            hexadecimal[hex_counter++] = 'd';
        }
        else if (ostatok == 14)
        {
            hexadecimal[hex_counter++] = 'e';
        }
        else if (ostatok == 15)
        {
            hexadecimal[hex_counter++] = 'f';
        }
        else
        {
            hexadecimal[hex_counter++] = ostatok + 48;
        }
    }
    hexadecimal[hex_counter] = '\0';
    return hex_counter;
}

int s21_big_add(s21_big_decimal value1, s21_big_decimal value2, s21_big_decimal *result)
{
    int ost = 0;
    int error = 0;
    for (int i = 0; i < 256; i++)
    {
        int bit1 = big_get_bit(value1, i);
        int bit2 = big_get_bit(value2, i);
        int res = bit1 + bit2 + ost;
        ost = res / 2;
        int bit3 = res % 2;
        big_set_bit(result, i, bit3);
        if (i == 255 && bit3 == 1)
            error = 1;
    }
    return error;
}
