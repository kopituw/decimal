#include "../s21_decimal.h"

int shift_value_len(s21_big_decimal decimal) {
  int len = 0;
  int bit_found = 1;
  for (int i = 255; i >= 0; i--) {
    if (!bit_found && big_get_bit(decimal, i)) {
      len = i + 1;
      bit_found = 1;
    }
  }
  return len;
}
