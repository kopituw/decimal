#include "../s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{

  s21_decimal remain = {0};
  s21_decimal temp = {0};
  int scale = 0;
  s21_div_basic(value_1, value_2, result, &remain);
  printf("ostatok? = %u with %d\n", remain.bit[0], get_scale(&remain));
  while (!is_zero(remain) && get_scale(&remain) < 28 && get_scale(&result) < 28)
  {
    s21_mul(remain, (s21_decimal){{10, 0, 0, 0}}, &remain);
    s21_mul(*result, (s21_decimal){{10, 0, 0, 0}}, result);
    s21_div_basic(remain, value_2, &temp, &remain);
    s21_add(*result, temp, result);
    set_scale(result, ++scale);
  }
  // printf("scale = %d\n", scale);

  return OK;
}

int s21_div_basic(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, s21_decimal *remain)
{
  if (is_zero(value_1) || is_zero(value_2))
    return DIVISION_BY_ZERO;

  if (!result)
    return NULL_POINTER_EXCEPTION;

  init_decimal(result);
  int sign = get_sign(&value_1) ^ get_sign(&value_2);
  set_sign(&value_1, 0);
  set_sign(&value_2, 0);
  int overflow = normalize(&value_1, &value_2);

  if (overflow == OK)
  {
    if (s21_is_equal(value_1, value_2))
    {
      set_bit(result, 0, 1);
    }
    else if (s21_is_less(value_1, value_2))
    {
      set_bit(result, 0, 0);
      *remain = value_1;
    }
    else
    {
      *remain = value_2;

<<<<<<<<< Temporary merge branch 1
      // while (s21_is_less_or_equal(huy, value_1))
      while (s21_is_less_or_equal(huy, value_1))
=========
      while (huy.bit[0] <= value_1.bit[0])
>>>>>>>>> Temporary merge branch 2
      {
        overflow = s21_add(*result, (s21_decimal){{1, 0, 0, 0}}, result);

        if (!overflow)
          overflow = s21_mul(value_2, *result, remain);
      }
      if (overflow == OK)
      {
        s21_sub(*remain, value_2, remain);
        s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
      }
      s21_sub(value_1, *remain, remain);

      set_sign(result, sign);
    }
  }

  return overflow;
}

int s21_remain(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{

  int overflow = OK;
  overflow = s21_div(value_1, value_2, result);
  // s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
  if (overflow == OK)
    overflow = s21_mul(*result, value_2, result);
  // set_scale(&tmp, get_scale(&value_1));
  if (overflow == OK)
    overflow = s21_sub(value_1, *result, result);
  return overflow;
}
