#include "../s21_decimal.h"

int mul10_stub(void) { return 0; }

void big_mul10(s21_big_decimal *dst) {
  uint64_t carry = 0;
  
  for (int i = 0; i < 8; i++) {
    uint64_t temp = (uint64_t)dst->b_bit[i] * 10 + carry;
    dst->b_bit[i] = (uint32_t)(temp & 0xFFFFFFFF);
    carry = temp >> 32;
  }
}