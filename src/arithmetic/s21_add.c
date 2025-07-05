#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int ret = OK;
    s21_big_decimal big_value_1 = {0}, big_value_2 = {0}, big_result = {0};
    int sign1 = get_sign(&value_1), sign2 = get_sign(&value_2), sign = 0;
    int exp1 = get_scale(&value_1), exp2 = get_scale(&value_2), exp = 0;

    if (result == NULL || ((exp1 > 28 || exp1 < 0) || (exp2 > 28 || exp2 < 0))) {
        ret = CONVERSION_ERROR;
    } else {
        null_decimal(result);

        big_normalize(value_1, value_2, &big_value_1, &big_value_2, &exp); // здесь сразу происходит и нормализация и конвертация
        s21_big_add(big_value_1, big_value_2, &big_result);

        ret = big_to_decimal(big_result, &result, &exp);

        set_scale(&result, exp);
        set_sign(&result, sign);
        removing_zeros(&result);
    }

    return ret;
}

int s21_big_add(s21_big_decimal value1, s21_big_decimal value2, s21_big_decimal *result) {
    int ost = 0;
    int error = 0;
    for(int i = 0; i < 256; i++) {
        int bit1 = big_get_bit(value1, i);
        int bit2 = big_get_bit(value2, i);
        int res = bit1 + bit2 + ost;
        ost = res / 2;
        int bit3 = res % 2;
        big_set_bit(result, i, bit3);
        if (i == 255 && bit3 == 1) error = 1; 
    }
    return error;
}
