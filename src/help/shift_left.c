#include "../s21_decimal.h"

int shift_left(s21_decimal *dec)
{
  int overflow = get_bit(*dec, 95);
  // if (!overflow)
  // {
  //   int bitTransfer_1 = get_bit(*dec, 31);
  //   int bitTransfer_2 = get_bit(*dec, 63);
  //   for (int x = 0; x < 3; x += 1)
  //     dec->bit[x] <<= 1;
  //   if (bitTransfer_1)
  //     set_bit(dec, 32, 1);
  //   if (bitTransfer_2)
  //     set_bit(dec, 64, 1);
  // }
  for (int i = 95; i >= 0; i--)
  {
    set_bit(dec, i, i ? get_bit(*dec, i - 1) : 0);
  }
  return overflow;
}

int get_bit_test(int src, int pos)
{
  if (src & (1 << pos))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void set_bit_test(int *dst, int pos, int value)
{
  if (value)
  {
    *dst |= (1 << pos);
  }
  else
  {
    *dst &= ~(1 << pos);
  }
}

// int shift_left_bit(s21_decimal *dec, int count)
// {
//   int bit_index = (count / 8) - 1;
//   // count /= 4;

//   int overflow = get_bit(*dec, 95);

//   for (int i = 3; i >= 0; i--)
//   {
//     for (int k = 31; k >= 0; k--)
//     {

//     }
//     set_bit(dec, i, i ? get_bit(*dec, i - 1) : 0);
//   }
//   return overflow;
// }

void shift_left_bit(s21_decimal *dec)
{
  int old = 0;
  for (int i = 0; i < 3; i++)
  {
    int new_bit = get_bit_test(dec->bit[i], 31);
    dec->bit[i] = dec->bit[i] << 1;
    set_bit(&(dec->bit[i]), 0, old);
    old = new_bit;
  }
}

int shift_left_offset(s21_decimal *dec, int offset)
{
  int overflow = 0;
  // printf("bit = %d\n", get_bit(*dec, 0));
  while (offset-- && !overflow)
  {
    overflow = shift_left(dec);
    // shift_left_bit(dec);
    // printf("over after shift left = %d\n", overflow);
  }
  // printf("ovrflw from shift = %d\n", overflow);
  return overflow;
}

// int shift_left_offset(s21_decimal *dec, int offset)
// {
//   int overflow = get_bit(*dec, 95);
//   if (!(get_bit(*dec, 95) + offset > 95))
//   {
//     for (int y = 0; y < offset; y++)
//     {
//       int bitTransfer_1 = get_bit(*dec, 31);
//       int bitTransfer_2 = get_bit(*dec, 63);
//       for (int x = 0; x < 3; x++)
//         dec->bit[x] <<= 1;
//       if (bitTransfer_1)
//         set_bit(dec, 32, 1);
//       if (bitTransfer_2)
//         set_bit(dec, 64, 1);
//     }
//   }
//   return overflow;
// }

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