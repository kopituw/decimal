#include "../s21_decimal.h"

int is_zero(s21_decimal value) {
  return !value.bit[0] && !value.bit[1] && !value.bit[2];
}

int big_is_zero(s21_big_decimal value) {
  return !value.b_bit[0] && !value.b_bit[1] && !value.b_bit[2] &&
         !value.b_bit[3] && !value.b_bit[4] && !value.b_bit[5] &&
         !value.b_bit[6] && !value.b_bit[7];
}