#include "../s21_decimal.h"

void remove_zero(s21_decimal *value) {
  s21_decimal ostatok = {{0, 0, 0, 0}};
  int tmp_scale = get_scale(value);

  set_scale(value, 0);
  s21_remain(*value, (s21_decimal){{10, 0, 0, 0}}, &ostatok);
  for (int i = tmp_scale; is_zero(ostatok) && i; i--)
  {
    s21_remain(*value, (s21_decimal){{10, 0, 0, 0}}, &ostatok);
    s21_div(*value, (s21_decimal){{10, 0, 0, 0}}, value);
  }
}