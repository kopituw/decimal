#include "../s21_decimal.h"

int get_sign(const s21_decimal *value) {
  uint32_t servicebit = value->bit[3];
  int sign = (servicebit >> 31) & 1;
  return sign;
}
