#include "../s21_decimal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int flag = 1;
  
  if (is_zero(value_1) && is_zero(value_2)) {
    flag = 1;
  } else if (get_sign(&value_1) != get_sign(&value_2)) {
    flag = 0;
  } else {
    flag = (value_1.bit[0] == value_2.bit[0] && 
            value_1.bit[1] == value_2.bit[1] && 
            value_1.bit[2] == value_2.bit[2] && 
            value_1.bit[3] == value_2.bit[3]);
  }
  
  return flag;
}

int s21_is_equal_modal(s21_decimal value_1, s21_decimal value_2) {
  return value_1.bit[2] == value_2.bit[2] && value_1.bit[1] == value_2.bit[1] &&
         value_1.bit[0] == value_2.bit[0];
}
