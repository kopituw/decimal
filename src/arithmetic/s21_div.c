#include "../s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
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
    }
    else
    {
      s21_decimal huy = value_2;

      // while (s21_is_less_or_equal(huy, value_1))
      while (s21_is_less_or_equal(huy, value_1))
      {
        // printf("! from div. 1:%u 2:%u %u res = %u\n", value_1.bit[0],
        // value_2.bit[0], huy.bit[0], result->bit[0]);
        overflow = s21_add(*result, (s21_decimal){{1, 0, 0, 0}}, result);
        if (!overflow)
          overflow = s21_mul(value_2, *result, &huy);
      }
      if (overflow == OK)
      {
        s21_sub(huy, value_2, &huy);
        s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
      }
      // printf("sign is %d\n", sign);
      set_sign(result, sign);
    }
  }

  return overflow;
}


int s21_remain(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{

  int overflow = OK;
  overflow = s21_div(value_1, value_2, result);
  if (overflow == OK)
    overflow = s21_mul(*result, value_2, result);
  if (overflow == OK)
    overflow = s21_sub(value_1, *result, result);

  return overflow;
}
