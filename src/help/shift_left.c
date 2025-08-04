#include "../s21_decimal.h"

int shift_left(s21_decimal *dec)
{
  int overflow = get_bit(*dec, 95);
  for (int i = 95; i >= 0; i--)
  {
    set_bit(dec, i, i ? get_bit(*dec, i - 1) : 0);
  }
  return overflow;
}

int shift_left_offset(s21_decimal *dec, int offset)
{
  int overflow = 0;
  while (offset--)
  {
    overflow = shift_left(dec);
  }
  return overflow;
}
