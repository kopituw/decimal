#include "../s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal c_result = {0};
    s21_big_decimal big_value_1 = {0}, big_value_2 = {0}, big_result = {0};
    int sign = 0, sign1 = get_sign(&value_1), sign2 = get_sign(&value_2);
    int scale = 0, scale1 = get_scale(&value_1), scale2 = get_scale(&value_2);
    int ret = OK;

    if (result == NULL || ((scale1 > 28 || scale1 < 0) || (scale2 > 28 || scale2 < 0)) || (correct_last_bits(value_1) != 1 || correct_last_bits(value_2))) {
        ret = CONVERSION_ERROR;
    } else {
        null_decimal(result);
        big_normalize(value_1, value_2, &big_value_1, &big_value_2, &scale);
        
        if (sign1 != sign2) {
            s21_big_add(big_value_1, big_value_2, &big_result);
            sign = sign1;
        } else {
            if (s21_big_is_equal(big_value_1, big_value_2)) {
                sign = 0;
            } else if (s21_big_is_greater(big_value_1, big_value_2)) {
                s21_big_sub(big_value_2, big_value_1, &big_result);
                sign = sign1;
            } else {
                s21_b_sub(big_value_2, big_value_1, &big_result);
                sign = !sign1; 
        }
    }
   if (scale1 == scale2) scale = scale1;

    ret = big_to_decimal(big_result, &c_result, &scale);

    if (ret == 1 && sign == 1) ret = NEGATIVE_INF;
    if (is_zero(value_1) && is_zero(value_2)) ret = 0;

    set_scale(&c_result, scale);
    set_sign(&c_result, sign);
    removing_zeros(&c_result);

    *result = c_result;

    return ret;
}