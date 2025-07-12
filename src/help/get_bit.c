#include "../s21_decimal.h"

int get_bit(const s21_decimal value, int bit) {
  int result = 0;

  if (bit / 32 < 4) {
    uint32_t mask = 1U << (bit % 32);
    result = value.bit[bit / 32] & mask;
  }
  return !!result;
}

int big_get_bit(const s21_big_decimal value, int bit) {
  int result = 0;

  if (bit / 32 < 9) {
    uint32_t mask = 1U << (bit % 32);
    result = value.b_bit[bit / 32] & mask;
  }
  return !!result;
}
