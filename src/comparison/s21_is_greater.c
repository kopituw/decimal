#include "../s21_decimal.h"

int s21_big_is_greater(s21_big_decimal value1, s21_big_decimal value2) {
  int result = 0;
  int is_comparing = 1;
  for (int i = 255; i >= 0 && is_comparing; i--) {
    int bit1 = big_get_bit(value1, i);
    int bit2 = big_get_bit(value2, i);
    if (bit1 > bit2) {
      result = 1;
      is_comparing = 0;
    }
    if (bit1 < bit2) {
      result = 0;
      is_comparing = 0;
    }
  }
  return result;
}

int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  int flag = 0;
  
  // Проверяем случай с нулями
  if (is_zero(dec1) && is_zero(dec2)) {
    flag = 0; // 0 не больше -0
  } else {
    // Если знаки разные
    int sign1 = get_sign(&dec1);
    int sign2 = get_sign(&dec2);
    
    if (sign1 != sign2) {
      flag = sign1 < sign2; // Положительное больше отрицательного
    } else {
      // Простое сравнение по битам
      int result = -1;
      for (int i = 3; i >= 0 && result == -1; i--) {
        if (dec1.bit[i] != dec2.bit[i]) {
          result = dec1.bit[i] > dec2.bit[i];
        }
      }
      if (result == -1) result = 0;
      
      // Если числа отрицательные, инвертируем результат
      if (sign1) {
        result = !result;
      }
      
      flag = result;
    }
  }
  
  return flag;
}

int s21_is_greater_modal(s21_decimal dec1, s21_decimal dec2) {
  int result = -1;

  for (int i = 2; i >= 0 && result == -1; i--) {
    if (dec1.bit[i] != dec2.bit[i]) result = dec1.bit[i] > dec2.bit[i];
  }
  if (result == -1) result = 0;

  return result;
}