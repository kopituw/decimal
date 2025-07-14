#include "../s21_decimal.h"

void null_decimal(s21_decimal *decimal) {
  decimal->bit[0] = 0;
  decimal->bit[1] = 0;
  decimal->bit[2] = 0;
  decimal->bit[3] = 0;
}

void big_null_decimal(s21_big_decimal *decimal) {
  decimal->b_bit[0] = 0;
  decimal->b_bit[1] = 0;
  decimal->b_bit[2] = 0;
  decimal->b_bit[3] = 0;
  decimal->b_bit[4] = 0;
  decimal->b_bit[5] = 0;
  decimal->b_bit[6] = 0;
  decimal->b_bit[7] = 0;
}