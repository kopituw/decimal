#include "../s21_decimal.h"

void remove_zero(s21_decimal *value) {
    s21_decimal c_value = *value;
    int scale = get_scale(&c_value);

    while (scale != 0) {
        s21_decimal quotient = {0};
        int remainder = div10(&c_value, &quotient);
        if (remainder == 0) {
            c_value = quotient;
            scale--;
        } else {
            break;
        }
    }

    set_scale(&c_value, scale);
    *value = c_value;
}