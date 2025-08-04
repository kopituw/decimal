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

// 

int s21_mul(s21_decimal a, s21_decimal b, s21_decimal *c) {
    if (!c) return NULL_POINTER_EXCEPTION;

    // Проверка на переполнение scale
    int scale_a = get_scale(&a);
    int scale_b = get_scale(&b);
    if (scale_a > 28 || scale_b > 28) return INF;
    if (scale_a < 0 || scale_b < 0) return NEGATIVE_INF;

    s21_decimal temp = {0};
    init_decimal(&temp);
    
    int sign_a = get_sign(&a);
    int sign_b = get_sign(&b);
    int result_sign = sign_a ^ sign_b;
    int result_scale = scale_a + scale_b;
    
    int overflow = OK;

    // Убираем scale для точного умножения
    set_scale(&a, 0);
    set_scale(&b, 0);

    for (int i = 0; i < 96 && overflow == OK; i++) {
        if (get_bit(a, i)) {
            s21_decimal tmp = b;
            overflow = shift_left_offset(&tmp, i);

            if (!overflow) {
                overflow = denya_add_basic(tmp, temp, &temp);
            }
        }
    }

    if (overflow == OK) {
        // Проверяем, не превысили ли максимальный scale (28)
        while (result_scale > 28 && !is_zero(temp)) {
            bank_round(&temp, 1);
            result_scale--;
        }
        
        if (result_scale > 28) {
            overflow = INF;
        } else {
            *c = temp;
            set_scale(c, result_scale);
            set_sign(c, result_sign);
        }
    }

    if (overflow) {
        overflow = (result_sign) ? NEGATIVE_INF : INF;
    }

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