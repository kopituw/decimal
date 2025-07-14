#include "../s21_decimal.h"

int get_scale(const s21_decimal *value) { return (char)(value->bit[3] >> 16); }
