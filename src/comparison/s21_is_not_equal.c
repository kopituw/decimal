#include "../s21_decimal.h"

// Заглушка, если нет реализации
int s21_big_is_equal(s21_big_decimal value1, s21_big_decimal value2) {
<<<<<<< HEAD
    // TODO: заменить на реальную реализацию
    for (int i = 0; i < 8; i++) {
        if (value1.b_bit[i] != value2.b_bit[i]) return 0;
    }
    return 1;
}

// int s21_big_is_not_equal(s21_big_decimal value_1, s21_big_decimal value_2)
// {
//     return !s21_big_is_equal(value_1, value_2);
// }

int s21_big_is_not_equal(s21_big_decimal value_1, s21_big_decimal value_2) {
  return !s21_big_is_equal(value_1, value_2);
=======
  // TODO: заменить на реальную реализацию
  for (int i = 0; i < 8; i++) {
    if (value1.b_bit[i] != value2.b_bit[i]) return 0;
  }
  return 1;
}

int s21_big_is_not_equal(s21_big_decimal value1, s21_big_decimal value2) {
  return !s21_big_is_equal(value1, value2);
>>>>>>> watchsan
}

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_equal(dec1, dec2);
}

int s21_is_not_equal_modal(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_equal_modal(dec1, dec2);
}