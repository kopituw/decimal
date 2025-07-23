#include "../s21_decimal.h"

<<<<<<< HEAD
int get_scale(const s21_decimal *value) {
    return (value->bit[3] >> 16) & 0xFF;
}
=======
int get_scale(const s21_decimal *value) { return (char)(value->bit[3] >> 16); }
>>>>>>> watchsan
