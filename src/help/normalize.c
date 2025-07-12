#include "../s21_decimal.h"

int normalize(s21_decimal *dec1, s21_decimal *dec2)
{
  int scale1 = get_scale(dec1), scale2 = get_scale(dec2);

  if (scale1 >= 28) {
    if (get_sign(dec1)) {
      return NEGATIVE_INF;
    } else {
      return INF;
    }
  }

  if (scale2 >= 28) {
    if (get_sign(dec2)) {
      return NEGATIVE_INF;
    } else {
      return INF;
    }
  }

  int min_scale = scale1 > scale2 ? scale2 : scale1, max_scale = scale1 > scale2 ? scale1 : scale2;
  s21_decimal *min_dec = min_scale == scale1 ? dec1 : dec2;
  s21_decimal *max_dec = max_scale == scale1 ? dec1 : dec2;

  while (min_scale != max_scale && min_scale < 28)
  {
    if (!s21_mul(*min_dec, (s21_decimal){{10, 0, 0, 0}}, min_dec))
    {
      min_scale++;
      set_scale(min_dec, min_scale);
    }
  }

  set_scale(min_dec, min_scale);
  set_scale(max_dec, min_scale);

  return OK;
}

int big_normalize(s21_decimal value1, s21_decimal value2, s21_big_decimal *big_value1,
                  s21_big_decimal *big_value2, int *scale)
{
  int exp1 = get_scale(&value1), exp2 = get_scale(&value2);
  int diff = exp1 - exp2;
  int c_exp = 0;

  s21_big_decimal c_big_value1 = *big_value1, c_big_value2 = *big_value2;

  decimal_to_big(value1, &c_big_value1);
  decimal_to_big(value2, &c_big_value2);

  if (diff > 0)
  {
    for (int i = 0; i < diff; i++)
      // big_mul10(&c_big_value2);
      c_exp = exp1;
  }
  else if (diff < 0)
  {
    for (int i = 0; i < -diff; i++)
      // big_mul10(&c_big_value1);
      c_exp = exp2;
  }

  *scale = c_exp;
  *big_value1 = c_big_value1;
  *big_value2 = c_big_value2;

  return 0;
}

void remove_zero(s21_decimal *value) {
    s21_decimal c_value = *value;
    int scale = get_scale(&c_value);

    while (scale != 0) {
        s21_decimal quotient = {0};
        int remainder = div10(&c_value, &quotient);
        if (remainder == 0) {
            c_value = quotient;
            scale--;
        } else {
            break;
        }
    }

    set_scale(&c_value, scale);
    *value = c_value;
}