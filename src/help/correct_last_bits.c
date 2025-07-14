#include "../s21_decimal.h"

int correct_last_bits(s21_decimal value) {
  int a = 0;
  for (int i = 96; i <= 111; i++) {
    if (get_bit(value, i) == 1) a++;
    // printf("%d\n", get_bit(value, i));
  }

  for (int i = 120; i <= 126; i++) {
    if (get_bit(value, i) == 1) a++;
  }

  return (a == 0) ? 1 : 0;
}