#include "../s21_decimal.h"

void decimal_to_big(s21_decimal value_1, s21_big_decimal *value_2) {
    value_2->b_bit[0] = value_1.bit[0];
    value_2->b_bit[1] = value_1.bit[1];
    value_2->b_bit[2] = value_1.bit[2];
}