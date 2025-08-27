#include "../s21_decimal.h"

void set_scale(s21_decimal *value, int scale) {
  int clearbit = ~(0xFF << 16);
  value->bit[3] &= clearbit;

  int maskbit = scale << 16;
  value->bit[3] |= maskbit;
}