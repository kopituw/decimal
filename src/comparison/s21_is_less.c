#include "../s21_decimal.h"

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  int flag = 0;
  
  if (is_zero(dec1) && is_zero(dec2)) {
    flag = 0;
  } else {
    int sign1 = get_sign(&dec1);
    int sign2 = get_sign(&dec2);
    
    if (sign1 != sign2) {
      flag = sign1 > sign2;
    } else {
      int result = -1;
      for (int i = 2; i >= 0 && result == -1; i--) {
        if (dec1.bit[i] != dec2.bit[i]) {
          result = dec1.bit[i] < dec2.bit[i];
        }
      }
      if (result == -1) result = 0;
      
      if (sign1) {
        result = !result;
      }
      
      flag = result;
    }
  }
  
  return flag;
}

int s21_is_less_modal(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_greater_modal(dec1, dec2);
}