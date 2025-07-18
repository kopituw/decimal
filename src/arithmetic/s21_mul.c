#include "../s21_decimal.h"

// void s21_big_mul(s21_big_decimal value1, s21_big_decimal value2,
// s21_big_decimal *result)
// {
//     for (int i = 0; i < 256; i++)
//     {
//         if (big_get_bit(value2, i))
//         {
//             s21_big_decimal temp = value1;
//             shift_left(&temp, i);
//             s21_big_add(*result, temp, result);
//         }
//     }
// }

int s21_mul(s21_decimal a, s21_decimal b, s21_decimal *c)
{
  if (!c)
    return NULL_POINTER_EXCEPTION;

  s21_decimal temp;
  init_decimal(&temp);
  int overflow = 0;
  int sign1 = get_sign(&a);
  int sign2 = get_sign(&b);

  for (int i = 0; i < 96 && !overflow; i++)
  {
    if (get_bit(a, i))
    {
      s21_decimal tmp = b;
      overflow = shift_left_offset(&tmp, i);

      if (!overflow)
        overflow = denya_add_basic(tmp, temp, &temp);
    }
  }
  if (!overflow)
    *c = temp;

  if (c->bit[0])
    set_sign(c, sign1 ^ sign2);

  if (overflow)
    overflow = (sign1 ^ sign2) ? NEGATIVE_INF : INF;

  return overflow;
}

int s21_mul_int(s21_decimal dec, int integer, s21_decimal *result)
{
  if (integer && dec.bit[0] && (get_sign(&dec) ^ (integer < 0)))
    set_sign(result, 1);

  if (integer < 0)
    integer = -integer;

  int counter = integer / 2;
  while (counter)
  {
    result->bit[0] += dec.bit[0] << 1;
    counter--;
  }
  if (integer && integer % 2 == 1)
    result->bit[0] += dec.bit[0];

  return 1;
}