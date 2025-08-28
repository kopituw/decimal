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

int s21_mul_new(s21_decimal a, s21_decimal b, s21_decimal *c)
{
  int scale_a = get_scale(&a);
  int scale_b = get_scale(&b);
  int total_work_bit_a = 0, total_work_bit_b = 0;

  for (int i = 0; i < 96; i++)
  {
    if (get_bit(a, i) && i > total_work_bit_a)
      total_work_bit_a = i;

    if (get_bit(b, i) && i > total_work_bit_b)
      total_work_bit_b = i;
  }
  int total_work_bit = total_work_bit_a + total_work_bit_b;
  int x = total_work_bit > 96 ? total_work_bit - 96 : 0;
  printf("!=!=!+1=1==!=!+! x = %d\n", x);
  printf("!=!=!+1=1==!=!+! total_work_bit_a = %d\n", total_work_bit_a);
  printf("!=!=!+1=1==!=!+! total_work_bit_b = %d\n", total_work_bit_b);
  int overflow = x > 28 ? 1 : OK;

  if (!overflow)
  {

    s21_decimal m = {0}, n = {0};
    set_scale(&a, x);
    set_scale(&b, x);
    s21_truncate(a, &m);
    s21_truncate(b, &n);
    // READY 1
    s21_decimal p = {0}, q = {0};
    s21_sub(a, m, &p);
    s21_sub(b, n, &q);

    // READY 2
    s21_decimal result = {0};

    printf("=== a\n");
    print_decimal(a);
    printf("=== b\n");
    print_decimal(b);
    printf("=== m\n");
    print_decimal(m);
    printf("=== n\n");
    print_decimal(n);
    printf("=== p\n");
    print_decimal(p);
    printf("=== q\n");
    print_decimal(q);
    /* code */
  }

  // for (int i = x; i; i--)
  // {
  //   s21_div(a, (s21_decimal){{10, 0, 0, 0}}, &m);
  // }

  return OK;
}

// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
// {
//   int error_flag = 0;

//   s21_big_decimal num_1, num_2, result_num;
//   s21_to_work_decimal(value_1, &num_1);
//   s21_to_work_decimal(value_2, &num_2);
//   if (!result)
//   {
//     error_flag = 1;
//   }
//   else
//   {
//     memset(result, 0, sizeof(s21_decimal));
//     s21_to_work_decimal(*result, &result_num);
//   }

//   int res_sign = get_sign(&value_1) ^ get_sign(&value_2);
//   // result_num.sign = num_1.sign ^ num_2.sign;

//   int res_scale = get_scale(&value_1) + get_scale(&value_2);
//   // result_num.scale = num_1.scale + num_2.scale;

//   for (int i = 0; !error_flag && i < 3; i++)
//   {
//     for (int j = 0; !error_flag && j < 3; j++)
//     {
//       result_num.b_bit[i + j] += num_1.b_bit[i] * num_2.b_bit[j];
//       if (s21_calc_get_overflow(&result_num))
//       {
//         error_flag = res_sign ? 2 : 1;
//       }
//     }
//   }

//   if (!error_flag)
//   {
//     error_flag = res_sign ? 2 : 1;
//   }
//   // if (!error_flag && normalize(&result_num))
//   // {
//   //   error_flag = res_sign ? 2 : 1;
//   // }

//   if (!error_flag)
//   {
//     s21_from_work_decimal(result_num, result);
//   }

//   return error_flag;
// }

// int s21_to_work_decimal(s21_decimal src, s21_big_decimal *dst)
// {
//   int error_flag = 0;
//   if (!dst)
//     error_flag = 1;
//   else
//   {
//     memset(dst, 0, sizeof(s21_big_decimal)); // Обнуляем результат

//     dst->b_bit[0] = (uint64_t)(uint32_t)src.bit[0];
//     dst->b_bit[1] = (uint64_t)(uint32_t)src.bit[1];
//     dst->b_bit[2] = (uint64_t)(uint32_t)src.bit[2];

//     uint32_t meta = (uint32_t)src.bit[3];
//     dst->scale = (meta & SCALE_MASK) >> 16;

//     dst->sign = ((src.bit[3] & S21_DECIMAL_SIGN_MASK) == 0) ? 0 : 1;
//   }

//   return error_flag;
// }

// int s21_from_work_decimal(s21_big_decimal src, s21_decimal *dst)
// {
//   int error_flag = 0;
//   if (!dst)
//     error_flag = 1;
//   else
//   {
//     memset(dst, 0, sizeof(s21_decimal));

//     dst->bit[0] = (int)(src.b_bit[0] & MAX4BITE);
//     dst->bit[1] = (int)(src.b_bit[1] & MAX4BITE);
//     dst->bit[2] = (int)(src.b_bit[2] & MAX4BITE);

//     // dst->bits[3] = ((int)src.scale << 16) & SC;

//     // dst->bits[3] = src.sign == 0 ? dst->bits[3] & ~MINUS : dst->bits[3] |
//     // MINUS;
//     uint32_t meta = 0;
//     meta |= ((uint32_t)src.scale << 16) & SCALE_MASK;
//     if (src.sign)
//       meta |= S21_DECIMAL_SIGN_MASK;
//     dst->bit[3] = (int)meta;
//   }
//   return error_flag;
// }

// // выравнивает число (использовать в процессе вычислений)
// // возможно придется переделать
// int s21_calc_get_overflow(s21_big_decimal *src)
// {
//   uint64_t overflow = 0;
//   for (int i = 0; i < 3; i++)
//   {
//     src->b_bit[i] += overflow;
//     overflow = src->b_bit[i] >> 31;
//     src->b_bit[i] &= MAX4BITE;
//   }

//   return overflow != 0;
// }

// int s21_mul(s21_decimal a, s21_decimal b, s21_decimal *c)
// {
//   if (!c)
//     return NULL_POINTER_EXCEPTION;

//   s21_decimal temp;
//   init_decimal(&temp);

//   int overflow = get_scale(&a) > 28 || get_scale(&b) > 28 ? INF : OK;

//   if (overflow == OK)
//     overflow = get_scale(&a) < 0 || get_scale(&b) < 0 ? NEGATIVE_INF : OK;

//   int sign1 = get_sign(&a);
//   int sign2 = get_sign(&b);

//   for (int i = 0; i < 96 && overflow == OK; i++)
//   {
//     if (get_bit(a, i))
//     {
//       s21_decimal tmp = b;
//       overflow = shift_left_offset(&tmp, i);

//       if (!overflow)
//         overflow = denya_add_basic(temp, tmp, &temp);
//     }
//   }
//   if (overflow == OK)
//     *c = temp;

//   if (!is_zero(*c))
//     set_sign(c, sign1 ^ sign2);

//   if (overflow)
//     overflow = (sign1 ^ sign2) ? NEGATIVE_INF : INF;

//   return overflow;
// }

int s21_mul(s21_decimal a, s21_decimal b, s21_decimal *c)
{
  if (!c)
    return NULL_POINTER_EXCEPTION;

  s21_decimal temp;
  init_decimal(&temp);
  int overflow = 0;

  for (int i = 0; i < 96 && !overflow; i++)
  {
    if (get_bit(a, i))
    {
      s21_decimal tmp = b;
      overflow = shift_left_offset(&tmp, i);

      if (!overflow)
      {

        printf("!--- tmp = %u.%u.%u\n", tmp.bit[0], tmp.bit[1], tmp.bit[2]);
        printf("!--- temp = %u.%u.%u\n", temp.bit[0], temp.bit[1], temp.bit[2]);

        overflow = denya_add_basic(tmp, temp, &temp);
      }
    }
  }
  if (!overflow)
    *c = temp;

  // int sign_1 = get_sign(&a), sign_2 = get_sign(&b);

  if (c->bit[0])
    set_sign(c, get_sign(&a) ^ get_sign(&b));

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
