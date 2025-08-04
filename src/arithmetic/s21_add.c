#include "../s21_decimal.h"

int s21_add(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result) {
    if (!result) return NULL_POINTER_EXCEPTION;
    
    int overflow = normalize(&dec_1, &dec_2);
    if (overflow != OK) return overflow;
    
    int sign_1 = get_sign(&dec_1);
    int sign_2 = get_sign(&dec_2);
    int scale = get_scale(&dec_1);
    
    init_decimal(result);
    
    if (sign_1 == sign_2) {
        overflow = denya_add_basic(dec_1, dec_2, result);
        set_sign(result, sign_1);
        if (overflow) {
            return sign_1 ? NEGATIVE_INF : INF;
        }
    } else {
        if (s21_is_greater_or_equal_modal(dec_1, dec_2)) {
            denya_sub_basic(dec_1, dec_2, result);
            set_sign(result, sign_1);
        } else {
            denya_sub_basic(dec_2, dec_1, result);
            set_sign(result, sign_2);
        }
    }
    
    set_scale(result, scale);
    return OK;
}

int denya_add_basic(s21_decimal dec1, s21_decimal dec2, s21_decimal *result)
{
  init_decimal(result);
  int mem = 0;
  for (int i = 0; i < 96; i++)
  {
    int dec1_bit = get_bit(dec1, i);
    int dec2_bit = get_bit(dec2, i);

    set_bit(result, i, dec1_bit ^ dec2_bit ^ mem);
    mem = (dec1_bit && dec2_bit) || (dec1_bit && mem) || (dec2_bit && mem);
  }
  return mem ? INF : OK;
}