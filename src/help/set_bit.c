#include "../s21_decimal.h"

void set_bit(s21_decimal *src, int bit, int value) {
    uint32_t mask = 1U << (bit % 32);
    if (bit / 32 < 4 && value) {
        src->bit[bit / 32] |= mask;
    } else if (bit / 32 < 4 && !value) {
        src->bit[bit / 32] &= ~mask;
    }
}

void big_set_bit(s21_big_decimal *src, int bit, int value) {
    uint32_t mask = 1U << (bit % 32);
    if (bit / 32 < 8 && value) {
        src->b_bit[bit / 32] |= mask;
    } else if (bit / 32 < 8 && !value) {
        src->b_bit[bit / 32] &= ~mask;
    }
}