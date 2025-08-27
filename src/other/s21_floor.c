#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (result == NULL)
    return NULL_POINTER_EXCEPTION;

  int sign = get_sign(&value);
  int scale = get_scale(&value);

  if (scale == 0) {
    *result = value;
    return OK;
  }

  s21_truncate(value, result);

  if (sign) {
    s21_decimal truncated_scaled = *result;
    set_scale(&truncated_scaled, scale);
    if (s21_is_not_equal(value, truncated_scaled)) {
      s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
    }
  }

  return OK;
}
