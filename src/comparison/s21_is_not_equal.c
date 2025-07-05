#include "../s21_decimal.h"

int s21_big_is_not_equal(s21_big_decimal value1, s21_big_decimal value2){
    return !s21_big_is_equal(value1, value2);
}
