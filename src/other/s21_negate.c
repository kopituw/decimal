#include "../s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
    set_sign(&value, !get_sign(&value));
    *result = value;
    
    return OK;
}