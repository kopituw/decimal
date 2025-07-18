#include "../s21_decimal.h"

int s21_big_is_less(s21_big_decimal value1, s21_big_decimal value2) {
  int result = 0;
  int is_comparing = 1;
  for (int i = 255; i >= 0 && is_comparing; i--) {
    int bit1 = big_get_bit(value1, i);
    int bit2 = big_get_bit(value2, i);
    if (bit1 < bit2) {
      result = 1;
      is_comparing = 0;
    }
    if (bit1 > bit2) {
      result = 0;
      is_comparing = 0;
    }
  }
  return result;
}

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_greater(dec1, dec2);
}

int s21_is_less_modal(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_greater_modal(dec1, dec2);
}