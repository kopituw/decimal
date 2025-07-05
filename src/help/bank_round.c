#include "../s21_decimal.h"

int bank_round(s21_big_decimal *value, s21_big_decimal value2, unsigned iter, unsigned flag) {
    s21_big_decimal c_value = *value, one = {0};
    one.b_bit[0] = 1;
    unsigned a = 0, b = 0;

    for (unsigned i = 0; i < iter; i++) {
        big_div10(&value, &a);
    }

    b = big_div10_rest(c_value); // дописать функцию господи боже какой треш

    if (a == 5 && flag > 0) a += 1;

    if (a == 5) {
        if (b % 2 != 0) {
            s21_big_add(c_value, one, &c_value);
        }
    } else if (a > 5) {
        s21_big_add(c_value, one, &c_value);
    }

    *value = c_value;

    return 0;
}