#include "../s21_decimal.h"

int div10(const s21_decimal *value, s21_decimal *quotient) {
  init_decimal(quotient);
  uint64_t rem = 0;
  for (int i = 2; i >= 0; --i) {
    uint64_t acc = ((uint64_t)rem << 32) | value->bit[i];
    quotient->bit[i] = (uint32_t)(acc / 10);
    rem = acc % 10;
  }
  quotient->bit[3] = value->bit[3];
  return (int)rem;
}

int big_div10(const s21_big_decimal *value, s21_big_decimal *quotient) {
  big_null_decimal(quotient);
  uint64_t rem = 0;
  for (int i = 7; i >= 0; --i) {
    uint64_t acc = ((uint64_t)rem << 32) | value->b_bit[i];
    quotient->b_bit[i] = (uint32_t)(acc / 10);
    rem = acc % 10;
  }
  return (int)rem;
}

void bank_round(s21_decimal *dec, unsigned count) {

  int scale = get_scale(dec);

  if (scale != 0 && count <= scale && dec) {

    int sign = get_sign(dec);
    int scale_res = scale - count;
    s21_decimal ten = {0}, one = {0}, two = {0}, two_res = {0};
    s21_from_int_to_decimal(10, &ten);
    s21_from_int_to_decimal(1, &one);
    s21_from_int_to_decimal(2, &two);
    s21_decimal half;
    s21_from_int_to_decimal(5, &half);
    set_scale(&half, scale_res + 1);

    s21_decimal ipart = {0};
    s21_truncate(*dec, &ipart);

    s21_decimal fpart = {0};
    s21_sub(*dec, ipart, &fpart);
    if (s21_is_greater_modal(fpart, half)) {
      set_scale(dec, count);
      s21_truncate(*dec, dec);
      denya_add_basic(*dec, one, dec);
    } else if (s21_is_equal_modal(fpart, half)) {
      s21_decimal temp_res = {0};
      set_scale(dec, 0);
      s21_div_basic(*dec, two, &temp_res, &two_res);
      if (s21_is_equal(one, two_res)) {
        set_scale(dec, scale);
        s21_truncate(*dec, dec);
        denya_add_basic(*dec, one, dec);
      }
    }
    set_sign(dec, sign);
    set_scale(dec, scale_res);
  }
}
