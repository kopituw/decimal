#include "../s21_decimal.h"

int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return s21_is_equal(dec1, dec2) || s21_is_greater(dec1, dec2);
}

int s21_is_greater_or_equal_modal(s21_decimal dec1, s21_decimal dec2) {
  return s21_is_equal_modal(dec1, dec2) || s21_is_greater_modal(dec1, dec2);
}