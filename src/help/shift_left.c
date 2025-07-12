#include "../s21_decimal.h"

int shift_left(s21_decimal *dec) {
  int overflow = get_bit(*dec, 95);
  for (int i = 95; i >= 0; i--) {
    set_bit(dec, i, i ? get_bit(*dec, i - 1) : 0);
  }
  return overflow;
}

int shift_left_offset(s21_decimal *dec, int offset) {
  int overflow = get_bit(*dec, 95);
  while (offset--) {
    shift_left(dec);
  }

  return overflow;
}

// void shift_left(s21_big_decimal* decimal, int shift_value) {
//     int current_len = shift_value_len(*decimal);
//     int can_shift = (shift_value > 0) && (current_len + shift_value <= 256);
//     if(can_shift) {
//         for (int i = 255 - shift_value; i >= 0; i--) {
//             int bit = big_get_bit(*decimal, i);
//             big_set_bit(decimal, i + shift_value, bit);
//         }
//         for (int i = 0; i < shift_value; i++){
//             big_set_bit(decimal, i, 0);
//         }
//     }
// }

// int big_shift_left(s21_big_decimal *value, int offset) {
//     int res = OK;
//     int lastbit = big_last_bit(*value);
//     if (lastbit + offset > 223) {
//         res = INF;
//     } else {
//         for (int i = 0; i < offset; i++) {
//             int bit31 = big_get_bit(*value, 31);
//             int bit63 = big_get_bit(*value, 63);
//             int bit95 = big_get_bit(*value, 95);
//             int bit127 = big_get_bit(*value, 127);
//             int bit159 = big_get_bit(*value, 159);
//             int bit191 = big_get_bit(*value, 191);
//             value->b_bit[0] <<= 1;
//             value->b_bit[1] <<= 1;
//             value->b_bit[2] <<= 1;
//             value->b_bit[3] <<= 1;
//             value->b_bit[4] <<= 1;
//             value->b_bit[5] <<= 1;
//             value->b_bit[6] <<= 1;

//             if (bit31) big_set_bit(value, 32, 1);
//             if (bit63) big_set_bit(value, 64, 1);
//         }
//     }
//     return res;
// }