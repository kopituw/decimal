#include "../s21_decimal.h"

int mul10(s21_decimal *value) {
  s21_decimal temp = *value;

  int overflow = shift_left(value);

  if (!overflow) {
    for (int i = 0; i < 3 && !overflow; i++) {
      overflow = shift_left(&temp);
    }

    if (!overflow) {
      overflow = s21_add(*value, temp, value);
    }
  }

  return overflow;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  if (result == NULL)
    return NULL_POINTER_EXCEPTION;

  int sign = get_sign(&value);
  int scale = get_scale(&value);

  if (scale == 0) {
    *result = value;
    return OK;
  }

  set_sign(&value, 0);

  s21_decimal half = {{5, 0, 0, 0}};
  set_scale(&half, 1); // 0.5

  for (int i = 0; i < scale - 1; i++) {
    mul10(&half);
  }
  set_scale(&half, scale);

  s21_decimal integer_part;
  s21_truncate(value, &integer_part);

  s21_decimal fractional;
  s21_sub(value, integer_part, &fractional);

  if (s21_is_greater_or_equal(fractional, half)) {
    s21_add(integer_part, (s21_decimal){{1, 0, 0, 0}}, &integer_part);
  }

  set_sign(&integer_part, sign);
  *result = integer_part;

  return OK;
}