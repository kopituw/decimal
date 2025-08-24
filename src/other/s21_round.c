#include "../s21_decimal.h"

int mul10(s21_decimal *value) {
    s21_decimal temp = *value;
    
    // Умножение на 2 (сдвиг влево на 1)
    int overflow = shift_left(value);
    
    if (!overflow) {
        // Умножение на 8 (сдвиг влево на 3)
        for (int i = 0; i < 3 && !overflow; i++) {
            overflow = shift_left(&temp);
        }
        
        // Сложение: value * 10 = value * 2 + value * 8
        if (!overflow) {
            overflow = s21_add(*value, temp, value);
        }
    }
    
    return overflow;
}

int s21_round(s21_decimal value, s21_decimal *result) {
    if (result == NULL) return NULL_POINTER_EXCEPTION;
    
    int sign = get_sign(&value);
    int scale = get_scale(&value);
    
    // Если масштаб 0, число уже целое
    if (scale == 0) {
        *result = value;
        return OK;
    }
    
    // Работаем с положительным числом
    set_sign(&value, 0);
    
    // Создаем decimal для числа 5 с нужным масштабом (0.5)
    s21_decimal half = {{5, 0, 0, 0}};
    set_scale(&half, 1);  // 0.5
    
    // Умножаем half на 10^(scale-1) чтобы получить 0.5 в нужном масштабе
    for (int i = 0; i < scale - 1; i++) {
        mul10(&half);
    }
    set_scale(&half, scale);
    
    // Получаем целую часть
    s21_decimal integer_part;
    s21_truncate(value, &integer_part);
    
    // Вычисляем дробную часть
    s21_decimal fractional;
    s21_sub(value, integer_part, &fractional);
    
    // Сравниваем дробную часть с 0.5
    if (s21_is_greater_or_equal(fractional, half)) {
        // Если дробная часть >= 0.5, добавляем 1 к целой части
        s21_add(integer_part, (s21_decimal){{1, 0, 0, 0}}, &integer_part);
    }
    
    // Восстанавливаем знак
    set_sign(&integer_part, sign);
    *result = integer_part;
    
    return OK;
}