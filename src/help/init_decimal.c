#include "../s21_decimal.h"

void init_decimal(s21_decimal *decimal)
{
  for (int i = 0; i < 128; i++)
    set_bit(decimal, i, 0);
}