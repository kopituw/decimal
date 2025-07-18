#include "../s21_decimal.h"

int s21_big_is_equal(s21_big_decimal value1, s21_big_decimal value2)
{
  int flag = 1;
  for (int i = 255; i >= 0 && flag; i--)
  {
    int bit1 = big_get_bit(value1, i);
    int bit2 = big_get_bit(value2, i);
    if (bit1 != bit2)
      flag = 0;
  }
  return flag;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2)
{
  return get_sign(&value_1) == get_sign(&value_2) && get_scale(&value_1) == get_scale(&value_2) &&
         value_1.bit[2] == value_2.bit[2] && value_1.bit[1] == value_2.bit[1] &&
         value_1.bit[0] == value_2.bit[0];
}

int s21_is_equal_modal(s21_decimal value_1, s21_decimal value_2)
{
  return value_1.bit[2] == value_2.bit[2] && value_1.bit[1] == value_2.bit[1] &&
         value_1.bit[0] == value_2.bit[0];
}
