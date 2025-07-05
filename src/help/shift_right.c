#include "../s21_decimal.h"

void shift_right(s21_decimal *value, int offset) {
    for (int i = 0; i < 3; i++) {
        value->bit[i] = (value->bit[i] >> offset) | (value->bit[i + 1] <<(32 - offset));
    }
    value->bit[3] = value->bit[3] >> offset;
}

void big_shift_right(s21_big_decimal *value, int offset) {
    for (int i = 0; i < 7; i++) {
        value->b_bit[i] = (value->b_bit[i] >> offset) | (value->b_bit[i + 1] << (32 - offset));
    }
    value->b_bit[7] = value->b_bit[7] >> offset;
}