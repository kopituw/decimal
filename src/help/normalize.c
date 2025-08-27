#include "../s21_decimal.h"

int normalize(s21_decimal *dec1, s21_decimal *dec2) {
  int scale1 = get_scale(dec1), scale2 = get_scale(dec2);
  int max_scale = (scale1 > scale2) ? scale1 : scale2;
  int min_scale = (scale1 < scale2) ? scale1 : scale2;
  int diff = max_scale - min_scale;

  if (max_scale > 28)
    return INF;
  if (min_scale < 0)
    return NEGATIVE_INF;

  for (int i = 0; i < diff; i++) {
    s21_decimal *target = (scale1 < scale2) ? dec1 : dec2;
    if (s21_mul(*target, (s21_decimal){{10, 0, 0, 0}}, target) != OK) {
      bank_round((scale1 > scale2) ? dec1 : dec2, 1);
      return normalize(dec1, dec2);
    }
    set_scale(target, min_scale + i + 1);
  }

  return OK;
}
