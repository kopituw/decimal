#include "../s21_decimal.h"

// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
// {
// }

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{

  s21_decimal remain = {0};
  s21_decimal temp = {0};
  int scale = 0;
  s21_div_basic(value_1, value_2, result, &remain);
  // printf("after 1st div res = %u with scl %d\n", result->bit[0], get_scale(result));
  // printf("ostatok? of %u \\ %u = %u with %d\n", value_1.bit[0], value_2.bit[0], remain.bit[0], get_scale(&remain));
  while (!is_zero(remain) && get_scale(&remain) < 28 && get_scale(result) < 28)
  {
    // printf("\nafter N div res = %u with scl %d\n", result->bit[0], get_scale(result));
    s21_mul(remain, (s21_decimal){{10, 0, 0, 0}}, &remain);
    s21_mul(*result, (s21_decimal){{10, 0, 0, 0}}, result);
    s21_div_basic(remain, value_2, &temp, &remain);
    printf("res = %u\n", result->bit[0]);
    printf("remain = %u\n", remain.bit[0]);
    // printf("after basic\n");
    denya_add_basic(*result, temp, result);
    set_scale(result, ++scale);
    // printf("scale = %d\n", scale);
  }
  // printf("itogo remain = %u\n", remain.bit[0]);

  return OK;
}

int s21_div_basic(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, s21_decimal *remain)
{
  if (is_zero(value_1) || is_zero(value_2))
    return DIVISION_BY_ZERO;

  if (!result)
    return NULL_POINTER_EXCEPTION;

  int overflow = normalize(&value_1, &value_2);
  init_decimal(result);
  int sign = get_sign(&value_1) ^ get_sign(&value_2);
  set_sign(&value_1, 0);
  set_sign(&value_2, 0);
  // printf("--- val1 = %u scl %d, val2 = %u scl %d\n", value_1.bit[0], get_scale(&value_1), value_2.bit[0], get_scale(&value_2));

  if (overflow == OK)
  {
    if (s21_is_equal(value_1, value_2))
    {
      set_bit(result, 0, 1);
    }
    else if (s21_is_less(value_1, value_2))
    {
      set_bit(result, 0, 0);
      *remain = value_1;
    }
    else
    {
      *remain = value_2;
      int count = 0;
      while (s21_is_less_or_equal(*remain, value_1) && !is_zero(*remain) && count < 27)
      {
        overflow = s21_add(*result, (s21_decimal){{1, 0, 0, 0}}, result);

        if (!overflow)
          overflow = s21_mul(value_2, *result, remain);
        // printf("res = %u\n", result->bit[0]);
        // printf("remain = %u\n", remain->bit[0]);

        // printf("huy = %d\n", remain->bit[0]);
        count++;
      }
      if (overflow == OK)
      {
        s21_sub(*remain, value_2, remain);
        s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
      }
      s21_sub(value_1, *remain, remain);

      set_sign(result, sign);
    }
  }
  else
  {
    printf("overflow = %d\n", overflow);
  }

  return overflow;
}

// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
// {
//   result->bit[0] = result->bit[1] = result->bit[2] = result->bit[3] = 0;
//   int res = 1;
//   int sign = 0;
//   int sign1 = get_sign(&value_1);
//   int sign2 = get_sign(&value_2);
//   if (sign1 != sign2)
//     sign = 1;
//   set_sign(&value_1, 0);
//   set_sign(&value_2, 0);
//   if (!value_2.bit[0] && !value_2.bit[1] && !value_2.bit[2])
//   {
//     res = 3;
//   }
//   else
//   {
//     s21_decimal tmp = {0};
//     set_scale(&value_1, 0);
//     set_scale(&value_2, 0);

//     for (int i = get_bit(value_1, 95); i >= 0; i--)
//     {
//       if (get_bit(value_1, i))
//         set_bit(&tmp, 0, 1);
//       if (s21_is_greater_or_equal(tmp, value_2))
//       {
//         s21_sub(tmp, value_2, &tmp);
//         if (i != 0)
//           shift_left(&tmp);
//         if (get_bit(value_1, i - 1))
//           set_bit(&tmp, 0, 1);
//         shift_left(result);
//         set_bit(result, 0, 1);
//       }
//       else
//       {
//         shift_left(result);
//         if (i != 0)
//           shift_left(&tmp);
//         if ((i - 1) >= 0 && get_bit(value_1, i - 1))
//           set_bit(&tmp, 0, 1);
//       }
//     }
//     res = 0;
//   }
//   set_sign(result, sign);
//   return res;
// }

// int s21_div1(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
//              s21_decimal *tmp)
// {
//   int res = 0;
//   for (int i = get_bit(value_1, 95); i >= 0; i--)
//   {
//     if (get_bit(value_1, i))
//       set_bit(&tmp, 0, 1);
//     if (s21_is_greater_or_equal(*tmp, value_2))
//     {
//       s21_sub(*tmp, value_2, &tmp);
//       if (i != 0)
//         shift_left(&tmp);
//       if (get_bit(value_1, i - 1))
//         set_bit(&tmp, 0, 1);
//       shift_left(result);
//       set_bit(result, 0, 1);
//     }
//     else
//     {
//       shift_left(result);
//       if (i != 0)
//         shift_left(&tmp);
//       if ((i - 1) >= 0 && get_bit(value_1, i - 1))
//         set_bit(&tmp, 0, 1);
//     }
//   }
//   return res;
// }

int s21_remain(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{

  int overflow = OK;
  overflow = s21_div(value_1, value_2, result);
  // s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
  if (overflow == OK)
    overflow = s21_mul(*result, value_2, result);
  // set_scale(&tmp, get_scale(&value_1));
  if (overflow == OK)
    overflow = s21_sub(value_1, *result, result);
  // printf("scl1 = %d scl2 = %d\n", get_scale(&value_1), get_scale(result));
  // // s21_add(value_1, *result, result);
  // printf("value_1 is %u.%u.%u with %d\n", value_1.bit[0], value_1.bit[1], value_1.bit[2], get_scale(&value_1));
  // // printf("tmp is %u.%u.%u with %d \n", tmp.bit[0], tmp.bit[1], tmp.bit[2], get_scale(&tmp));
  // printf("remain is %u.%u.%u with %d \n", result->bit[0], result->bit[1], result->bit[2], get_scale(result));

  return overflow;
}

// int s21_remain(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
// {
//   if (is_zero(value_1) || is_zero(value_2))
//     return DIVISION_BY_ZERO;

//   if (!result)
//     return NULL_POINTER_EXCEPTION;

//   init_decimal(result);
//   int overflow = normalize(&value_1, &value_2);

//   // overflow = normalize(&value_1, result);
//   // printf("scl1 = %d scl2 = %d\n", get_scale(&value_1), get_scale(&value_2));

//   if (overflow == OK)
//   {
//     if (s21_is_equal(value_1, value_2))
//     {
//       set_bit(result, 0, 1);
//     }
//     else if (s21_is_less(value_1, value_2))
//     {
//       set_bit(result, 0, 0);
//     }
//     else
//     {
//       s21_decimal tmp = value_2;

//       // while (s21_is_less_or_equal(tmp, value_1))
//       while (tmp.bit[0] <= value_1.bit[0])
//       {
//         // printf("! from div. 1:%u 2:%u %u res = %u\n", value_1.bit[0],
//         // value_2.bit[0], tmp.bit[0], result->bit[0]);
//         overflow = s21_add(*result, (s21_decimal){{1, 0, 0, 0}}, result);
//         if (!overflow)
//           overflow = s21_mul(value_2, *result, &tmp);
//       }

//       // printf("tmp is %u.%u.%u\n", tmp.bit[0], tmp.bit[1], tmp.bit[2]);
//       if (overflow == OK)
//       {
//         // s21_sub(*result, tmp, result);
//         s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
//         s21_mul(*result, value_2, &tmp);
//         set_scale(&tmp, get_scale(&value_1));

//         s21_sub(value_1, tmp, result);
//         printf("scl1 = %d scl2 = %d\n", get_scale(&value_1), get_scale(result));
//         // s21_add(value_1, *result, result);
//         printf("value_1 is %u.%u.%u with %d\n", value_1.bit[0], value_1.bit[1], value_1.bit[2], get_scale(&value_1));
//         printf("tmp is %u.%u.%u with %d \n", tmp.bit[0], tmp.bit[1], tmp.bit[2], get_scale(&tmp));
//         printf("remain is %u.%u.%u with %d \n", result->bit[0], result->bit[1], result->bit[2], get_scale(result));

//         // normalize(&value_1, tmp);
//       }
//     }
//   }

//   return overflow;
// }

// int s21_remain(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//   if (is_zero(value_1) || is_zero(value_2)) return DIVISION_BY_ZERO;

//   if (!result) return NULL_POINTER_EXCEPTION;

//   init_decimal(result);
//   int overflow = normalize(&value_1, &value_2);

//   if (overflow == OK) {
//     if (s21_is_equal(value_1, value_2)) {
//       set_bit(result, 0, 1);
//     } else if (s21_is_less(value_1, value_2)) {
//       set_bit(result, 0, 0);
//     } else {
//       s21_decimal huy = value_2;

//       // while (s21_is_less_or_equal(huy, value_1))
//       while (huy.bit[0] <= value_1.bit[0]) {
//         overflow = s21_add(*result, (s21_decimal){{1, 0, 0, 0}}, result);
//         if (!overflow) overflow = s21_mul(value_2, *result, &huy);
//       }
//       if (overflow == OK) {
//         s21_sub(huy, value_2, &huy);
//         s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
//         s21_mul(value_2, *result, result);
//         s21_sub(value_1, *result, result);
//         // printf("! from div. 1:%u 2:%u %u res = %u\n", value_1.bit[0],
//         // value_2.bit[0], huy.bit[0], result->bit[0]);
//       }
//     }
//   }

//   return overflow;
// }

// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//     s21_decimal c_result = {0};
//     int ret = OK;
//     int scale = 0, scale_rem = 0, rem = 0;
//     int sign1 = get_sign(&value_1), sign2 = get_sign(&value_2), sign = 0;
//     int scale1 = get_scale(&value_1), scale2 = get_scale(&value_2);
//     s21_big_decimal big_value_1 = {0}, big_value_2 = {0}, big_result = {0},
//     result_rem = {0};

//     if (result == NULL || ((scale1 > 28 || scale < 0) || (scale2 > 28 ||
//     scale2 < 0)) || (correct_last_bits(value_1) != 1 ||
//     correct_last_bits(value_2) != 1)) {
//         ret = CONVERSION_ERROR;
//     } else if (is_zero(value_2)) {
//         ret = DIVISION_BY_ZERO;
//     } else {
//         s21_decimal c_value_1 = value_1;

//         if (sign1 != sign2) {
//             sign = 1;
//         }

//         null_decimal(result);
//         big_normalize(value_1, value_2, &big_value_1, &big_value_2, &scale);
//         rem = s21_big_div(big_value_1, big_value_2, &big_result);

//         if (rem) {
//             scale_rem = remains(big_result, big_value_1, big_value_2,
//             &result, scale);
//         }

//         scale = scale_rem;

//         for (int i = scale_rem; i > 0; i--) {
//             big_mul10(&big_result);
//         }

//         s21_big_add(big_result, result_rem, &big_result);
//         ret = big_to_decimal(big_result, &c_result, &scale);

//         if (ret == 1 && sign == 1) {
//             ret = NEGATIVE_INF;
//         }

//         if (is_zero(value_1)) {
//             ret = OK;
//         }

//         if (!is_zero(c_value_1) && is_zero(value_1) && is_zero(c_result)) {
//             ret = NEGATIVE_INF;
//         }

//         set_scale(&c_result, scale);
//         set_sign(&c_result, sign);

//         *result = c_result;
//     }

//     return ret;
// }

// int s21_big_div(s21_big_decimal value_1, s21_big_decimal value_2,
// s21_big_decimal *result) {
//     big_null_decimal(result);

//     s21_big_decimal c_result = {0}, sub = value_1;
//     int bit_value_1 = 0, bit_value_2 = 0, rem = 0;

//     for (int i = 0; i < 256; i++) {
//         if (big_get_bit(value_1, i) == 1) {
//             bit_value_1 = i;
//         }
//         if (big_get_bit(value_2, i) == 1) {
//             bit_value_2 = i;
//         }
//     }

//     int i = bit_value_1 - bit_value_2;
//     int flag = 0;

//     for (int y = i; y > 0; y--) {
//         big_shift_right(&sub, 1);
//     }

//     while (i >= 0) {
//         // дописать
//         if (s21_big_is_greater(sub, value_2) || s21_big_is_equal(sub,
//         value_2)) {
//             big_shift_left(&c_result, 1); // сдвигаем влево если саб больше
//             или равно занчению big_set_bit(&c_result, 0, 1); // устанавливаем
//             в младший бит результат s21_big_sub(sub, value_2, &sub); //
//             дописать flag = 1;
//         } else {
//             big_shift_left(&c_result, 1); // сдвигаем результат влево
//         }
//         big_shift_left(&sub, 1);
//         if (i >= 0) {
//             big_set_bit(&sub, 0, big_get_bit(value_1, i));
//         }
//         i--;
//     }
//     if (big_is_zero(value_1)) rem = 0;
//     else if (s21_big_is_greater(value_2, value_1)) rem = 1;
//     else if (!big_is_zero(sub)) rem = 1;
//     else if (sub.b_bit[0] == 5) rem = 5;

//     *result = c_result;

//     return rem;
// }

// int remains(s21_big_decimal c_result, s21_big_decimal value_1,
// s21_big_decimal value_2, s21_big_decimal *result, int scale) {
//     s21_big_decimal src = {0}, c_res = {0};
//     int scale_rem = 0, rem = 1;

//     while ((rem != 0) && (scale + scale_rem == 40)) {
//         s21_big_mul(c_result, value_2, &src);
//         s21_big_sub(value_1, src, &src);
//         big_mul10(&src);
//         value_1 = src;
//         rem = s21_big_div(src, value_2, &src);

//         if (!big_is_zero(c_res)) {
//     big_mul10(&c_res);
//     s21_big_add(c_res, src, &c_res);
//     } else {
//     c_res = src;
//     }

//         c_result = src;
//         big_null_decimal(&src);
//         scale_rem++;
//     }
//     *result = c_res;
//     return scale_rem;
// }