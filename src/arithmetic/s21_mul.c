#include "../s21_decimal.h"

void s21_big_mul(s21_big_decimal value1, s21_big_decimal value2, s21_big_decimal *result) {
    for (int i = 0; i < 256; i++) {
        if (big_get_bit(value2, i)) {
            s21_big_decimal temp = value1;
            shift_left(&temp, i);
            s21_big_add(*result, temp, result);
        }
    }
}
