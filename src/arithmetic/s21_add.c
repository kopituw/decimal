#include "../s21_decimal.h"

void init_decimal(s21_decimal *decimal)
{
  for (int i = 0; i < 128; i++)
    set_bit(decimal, i, 0);
}

int s21_round(s21_decimal value, s21_decimal *result)
{
  if (result == NULL)
    return NULL_POINTER_EXCEPTION;

  // printf("VALUE from round %u\n", value.bit[0]);

  int count = get_scale(&value), overflow = OK;
  s21_decimal fpart_size = {1, 0, 0, 0}, five = {5, 0, 0, 0};

  set_scale(&value, 0);
  while (count-- > 0)
  {
    s21_mul(fpart_size, (s21_decimal){10, 0, 0, 0}, &fpart_size);
  }
  s21_mul(fpart_size, five, &five);
  s21_div(five, (s21_decimal){{10, 0, 0, 0}}, &five);
  s21_decimal remain = {0};
  s21_remain(value, fpart_size, &remain);
  overflow = s21_div(value, fpart_size, &value);

  // printf("five = %u with %d\n", five.bit[0], get_scale(&five));
  // printf("oink sc1: %d sc2: %d\n", );
  // printf("ostatok = %s%u with %d\n", get_sign(&remain) ? "-" : "+", remain.bit[0], get_scale(&remain));
  if (s21_is_greater_or_equal_modal(remain, five) && !is_zero(remain))
  {
    // printf("oink before add %u\n", value.bit[0]);
    overflow = denya_add_basic(value, (s21_decimal){{1, 0, 0, 0}}, &value);
    // printf("oink after add %u\n", value.bit[0]);
  }

  *result = value;
  set_scale(result, 0);
  return overflow;
}

int s21_floor(s21_decimal value, s21_decimal *result)
{
  int overflow = OK;
  s21_decimal half = {{5, 0, 0, 0}};

  set_scale(&half, get_scale(&value));

  s21_sub(value, half, &value);
  s21_round(value, result);
  // }
  // else
  // {
  //   printf("plus\n");
  //   int count = get_scale(&value);
  //   while (count-- > 0)
  //   {
  //     overflow = s21_div(value, ten, &value);
  //   }
  //   set_scale(&value, 0);
  //   *result = value;
  // }

  return overflow;
}

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
        if (overflow) {
            return sign_1 ? NEGATIVE_INF : INF;
        }
    } else {
        if (s21_is_greater_or_equal_modal(dec_1, dec_2)) {
            denya_sub_basic(dec_1, dec_2, result);
            set_sign(result, sign_1);
        } else {
            denya_sub_basic(dec_2, dec_1, result);
            set_sign(result, sign_2);
        }
    }
    
    set_scale(result, scale);
    return OK;
}

int denya_add_basic(s21_decimal dec1, s21_decimal dec2, s21_decimal *result)
{
  init_decimal(result);
  // printf("huy\n");
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