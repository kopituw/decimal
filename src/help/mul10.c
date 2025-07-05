#include "../s21_decimal.h"

// int mul10(s21_decimal *dst) {
//     int flag = OK;
//     s21_decimal c_dst = *dst, c_dst2 = *dst, res = {0};

//     if (dst != NULL) {
//         shift_left(&c_dst, 3);
//         shift_left(&c_dst2, 1);
//         // s21_add(c_dst, c_dst2, &res); достать когда появится сложение
//         dst->bit[0] = (dst->bit[0] << 3) + (dst->bit[0] << 1); // временная некорректная мера(((
//     } else if (dst != NULL && dst == 0) {
//         flag = DIVISION_BY_ZERO;
//     } else {
//         flag = CONVERSION_ERROR;
//     }
//     null_decimal(dst);

//     *dst = res;

//     return flag;
// }

void big_mul10(s21_big_decimal *value) {
    s21_big_decimal c_value = *value, c_value2 = *value, res = {0};
    shift_left(&c_value, 3);
    shift_left(&c_value2, 1);
    s21_big_add(c_value, c_value2, &res);
    *value = res;
}