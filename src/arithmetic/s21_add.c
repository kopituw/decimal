#include "../s21_decimal.h"

// void norm_decimal(s21_decimal dec1, s21_decimal dec2)
// {
//     int scale1 = get_scale(&dec1), scale2 = get_scale(&dec2);

//     int min_scale = scale1 > scale2 ? scale2 : scale1, max_scale = scale1 >
//     scale2 ? scale2 : scale1; s21_decimal min_dec = min_scale == scale1 ?
//     dec1 : dec2, max_dec = max_scale == scale1 ? dec1 : dec2; s21_decimal ten
//     = {{0, 0, 0, 10}};

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
  int overflow = normalize(&dec_1, &dec_2), sign_1 = get_sign(&dec_1),
      sign_2 = get_sign(&dec_2), scale = get_scale(&dec_1);

  if (sign_1 ^ sign_2 && overflow == OK)
  {
    if (s21_is_greater_or_equal_modal(dec_1, dec_2))
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
  else if (overflow == OK)
  {
    overflow = denya_add_basic(dec_1, dec_2, result);

    if (result->bit[0])
      set_sign(result, sign_1);
    if (overflow == INF && sign_1 && sign_2)
      overflow = NEGATIVE_INF;
  }

  if (overflow != OK && scale > 0 && scale <= 28)
  {
    bank_round(&dec_1, 1);
    bank_round(&dec_2, 1);
    overflow = s21_add(dec_1, dec_2, result);
  }
  else
  {
    set_scale(result, scale);
  }

  return overflow;
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
  return mem ? INF : OK;
}