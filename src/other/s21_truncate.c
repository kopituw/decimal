#include "../s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
    if (result == NULL) {
        return NULL_POINTER_EXCEPTION; // Код 5
    }

    init_decimal(result);
    
    int sign = get_sign(&value);
    int scale = get_scale(&value);
    
    if (scale == 0) {
        *result = value;
        return OK;
    }

    s21_big_decimal big_val;
    big_null_decimal(&big_val);
    decimal_to_big(value, &big_val);

    for (int i = 0; i < scale; i++) {
        if (big_is_zero(big_val)) break;
        
        s21_big_decimal quotient;
        big_null_decimal(&quotient);
        big_div10(&big_val, &quotient);
        big_val = quotient;
    }

    // Проверка переполнения
    for (int i = 3; i < 8; i++) {
        if (big_val.b_bit[i] != 0) {
            return CONVERSION_ERROR;
        }
    }

    result->bit[0] = big_val.b_bit[0];
    result->bit[1] = big_val.b_bit[1];
    result->bit[2] = big_val.b_bit[2];
    
    set_scale(result, 0);
    set_sign(result, sign);
    
    return OK;
}