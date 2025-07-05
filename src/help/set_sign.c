#include "../s21_decimal.h"

void set_sign(s21_decimal *value, int sign) {
    if (sign != 0) {
        value->bit[3] |= (S21_DECIMAL_SIGN_MASK);
    } else {
        value->bit[3] &= ~(S21_DECIMAL_SIGN_MASK);
    }
}
