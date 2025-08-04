#include "../s21_decimal.h"

// 

int is_max_decimal(s21_decimal dec) {
    return dec.bit[0] == 0xFFFFFFFF && 
           dec.bit[1] == 0xFFFFFFFF && 
           dec.bit[2] == 0xFFFFFFFF;
}

int s21_sub(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result) {
    if (!result) return NULL_POINTER_EXCEPTION;
    
    int sign1 = get_sign(&dec_1);
    int sign2 = get_sign(&dec_2);
    
    // Нормализация (убедитесь, что она не меняет знаки)
    int overflow = normalize(&dec_1, &dec_2);
    if (overflow != OK) return overflow;
    
    int scale = get_scale(&dec_1);
    init_decimal(result);
    
    if (sign1 != sign2) {
        // Если знаки разные - фактически сложение
        overflow = denya_add_basic(dec_1, dec_2, result);
        set_sign(result, sign1);
        
        // Обработка переполнения при сложении
        if (overflow) {
            return sign1 ? NEGATIVE_INF : INF;
        }
    } 
    else {
        // Сравнение по модулю
        int cmp = s21_is_greater_or_equal_modal(dec_1, dec_2);
        
        if (cmp) {
            overflow = denya_sub_basic(dec_1, dec_2, result);
            set_sign(result, sign1);
        } else {
            overflow = denya_sub_basic(dec_2, dec_1, result);
            set_sign(result, !sign1);
            
            // Особый случай: вычитание из минимального значения
            if (is_max_decimal(dec_2)) {
                return NEGATIVE_INF;
            }
        }
    }
    
    set_scale(result, scale);
    return overflow;
}

int denya_sub_basic(s21_decimal dec1, s21_decimal dec2, s21_decimal *result) {
  int overflow = OK;
  init_decimal(result);

  if (s21_is_less_modal(dec1, dec2)) overflow = NEGATIVE_INF;

  for (int i = 0; i < 96 && overflow == OK; i++) {
    int dec1_bit = get_bit(dec1, i);
    int dec2_bit = get_bit(dec2, i);
    set_bit(result, i, dec1_bit ^ dec2_bit);

    if (!dec1_bit && dec2_bit) {
      int j = i + 1;
      int dec1_bit_new = 0;
      while (!(dec1_bit_new = get_bit(dec1, j))) {
        set_bit(&dec1, j, 1);
        j++;
      }

      set_bit(&dec1, j, 0);
    }
  }
  return overflow;
}