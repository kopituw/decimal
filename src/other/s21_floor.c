#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
    if (result == NULL) return NULL_POINTER_EXCEPTION;
    
    int sign = get_sign(&value);
    int scale = get_scale(&value);
    
    // Если масштаб 0, число уже целое
    if (scale == 0) {
        *result = value;
        return OK;
    }
    
    // Получаем целую часть отбрасыванием дробной
    s21_truncate(value, result);
    
    // Для отрицательных чисел: если была дробная часть, вычитаем 1
    if (sign) {
        s21_decimal truncated_scaled = *result;
        set_scale(&truncated_scaled, scale); // Устанавливаем исходный масштаб для сравнения
        
        if (s21_is_not_equal(value, truncated_scaled)) {
            s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
        }
    }
    
    return OK;
}

