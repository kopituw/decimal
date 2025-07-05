#include "../s21_decimal.h"

void remove_zero(s21_decimal *value) {
    s21_decimal c_value = *value;
    int scale = get_scale(&c_value);

    while (scale != 0) {
        if (div10_dec(c_value) == 0) {
            unsigned b = 0;
            div10(&c_value, &b);
            scale --;
        }
    }

    set_scale(&c_value, scale);

    *value = c_value;
}