#include "../s21_decimal.h"

int get_scale(const s21_decimal *value) {
    return (value->bit[3] >> 16) & 0xFF;
}
