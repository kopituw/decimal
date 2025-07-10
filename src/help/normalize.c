#include "../s21_decimal.h"

int normalize(s21_big_decimal *big_value1, s21_big_decimal *big_value2, s21_decimal value1, s21_decimal value2)
{
  int exp1 = get_scale(&value1);
  // printf("exp1 = %d\n", exp1);
  int exp2 = get_scale(&value2);
  // printf("exp2 = %d\n", exp2);
  int diff = exp1 - exp2;
  int c_exp = 0;

  if (diff > 0)
  {
    // for (int i = 0; i < diff; i++) big_mul10(big_value2);
    c_exp = exp1;
  }
  else if (diff < 0)
  {
    // for (int i = 0; i < -diff; i++) big_mul10(big_value1);
    c_exp = exp2;
  }
  return c_exp;
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