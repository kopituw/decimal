#include "../s21_decimal.h"

void init_decimal(s21_decimal *decimal)
{
  for (int i = 0; i < 128; i++)
    set_bit(decimal, i, 0);
}
int s21_truncate(s21_decimal value, s21_decimal *result)
{
  if (result == NULL)
  {
    return NULL_POINTER_EXCEPTION; // Код 5
  }

  init_decimal(result);

  int sign = get_sign(&value);
  int scale = get_scale(&value);

  if (scale == 0)
  {
    *result = value;
    return OK;
  }

  s21_big_decimal big_val;
  big_null_decimal(&big_val);
  decimal_to_big(value, &big_val);

  for (int i = 0; i < scale; i++)
  {
    if (big_is_zero(big_val))
      break;

    s21_big_decimal quotient;
    big_null_decimal(&quotient);
    big_div10(&big_val, &quotient);
    big_val = quotient;
  }

  // Проверка переполнения
  for (int i = 3; i < 8; i++)
  {
    if (big_val.b_bit[i] != 0)
    {
      return CONVERSION_ERROR;
    }
  }

  result->bit[0] = big_val.b_bit[0];
  result->bit[1] = big_val.b_bit[1];
  result->bit[2] = big_val.b_bit[2];

  set_scale(result, 0);
  set_sign(result, sign);

  return OK;
}

int mul10(s21_decimal *value)
{
  s21_decimal temp = *value;

  // Умножение на 2 (сдвиг влево на 1)
  int overflow = shift_left(value);

  if (!overflow)
  {
    // Умножение на 8 (сдвиг влево на 3)
    for (int i = 0; i < 3 && !overflow; i++)
    {
      overflow = shift_left(&temp);
    }

    // Сложение: value * 10 = value * 2 + value * 8
    if (!overflow)
    {
      overflow = s21_add(*value, temp, value);
    }
  }

  return overflow;
}
int s21_round(s21_decimal value, s21_decimal *result)
{
  if (result == NULL)
    return NULL_POINTER_EXCEPTION;

  int sign = get_sign(&value);
  int scale = get_scale(&value);

  // Если масштаб 0, число уже целое
  if (scale == 0)
  {
    *result = value;
    return OK;
  }

  // Работаем с положительным числом
  set_sign(&value, 0);

  // Создаем decimal для числа 5 с нужным масштабом (0.5)
  s21_decimal half = {{5, 0, 0, 0}};
  set_scale(&half, 1); // 0.5

  // Умножаем half на 10^(scale-1) чтобы получить 0.5 в нужном масштабе
  for (int i = 0; i < scale - 1; i++)
  {
    mul10(&half);
  }
  set_scale(&half, scale);

  // Получаем целую часть
  s21_decimal integer_part;
  s21_truncate(value, &integer_part);

  // Вычисляем дробную часть
  s21_decimal fractional;
  s21_sub(value, integer_part, &fractional);

  // Сравниваем дробную часть с 0.5
  if (s21_is_greater_or_equal(fractional, half))
  {
    // Если дробная часть >= 0.5, добавляем 1 к целой части
    s21_add(integer_part, (s21_decimal){{1, 0, 0, 0}}, &integer_part);
  }

  // Восстанавливаем знак
  set_sign(&integer_part, sign);
  *result = integer_part;

  return OK;
}

int s21_floor(s21_decimal value, s21_decimal *result)
{
  int overflow = OK;
  s21_decimal half = {{5, 0, 0, 0}};

  set_scale(&half, get_scale(&value));

  s21_sub(value, half, &value);
  s21_round(value, result);
  // }
  // else
  // {
  //   printf("plus\n");
  //   int count = get_scale(&value);
  //   while (count-- > 0)
  //   {
  //     overflow = s21_div(value, ten, &value);
  //   }
  //   set_scale(&value, 0);
  //   *result = value;
  // }

  return overflow;
}

// void norm_decimal(s21_decimal dec1, s21_decimal dec2)
// {
//     int scale1 = get_scale(&dec1), scale2 = get_scale(&dec2);

//     int min_scale = scale1 > scale2 ? scale2 : scale1, max_scale = scale1 >
//     scale2 ? scale2 : scale1; s21_decimal min_dec = min_scale == scale1 ?
//     dec1 : dec2, max_dec = max_scale == scale1 ? dec1 : dec2; s21_decimal ten
//     = {{0, 0, 0, 10}};

//     while (min_scale != max_scale)
//     {
//         if (min_scale < 28 && !s21_mul(min_dec, ten, &min_dec))
//         {
//             min_scale++;
//             set_scale(&min_dec, min_scale);
//         }
//     }

//     set_scale(&min_dec, min_scale);
//     set_scale(&max_dec, min_scale);
// }

int s21_add(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result)
{
  int overflow = normalize(&dec_1, &dec_2), sign_1 = get_sign(&dec_1),
      sign_2 = get_sign(&dec_2), scale = get_scale(&dec_1);

  if (sign_1 ^ sign_2 && overflow == OK)
  {
    if (s21_is_greater_or_equal_modal(dec_1, dec_2))
    {
      denya_sub_basic(dec_1, dec_2, result);

      if (result->bit[0])
        set_sign(result, sign_1);
    }
    else
    {
      denya_sub_basic(dec_2, dec_1, result);
      if (result->bit[0])
        set_sign(result, sign_2);
    }
  }
  else if (overflow == OK)
  {
    overflow = denya_add_basic(dec_1, dec_2, result);

    if (result->bit[0])
      set_sign(result, sign_1);
    if (overflow == INF && sign_1 && sign_2)
      overflow = NEGATIVE_INF;
  }

  if (overflow != OK && scale > 0 && scale <= 28)
  {
    bank_round(&dec_1, 1);
    bank_round(&dec_2, 1);
    overflow = s21_add(dec_1, dec_2, result);
  }
  else
  {
    set_scale(result, scale);
  }

  return overflow;
}

int denya_add_basic(s21_decimal dec1, s21_decimal dec2, s21_decimal *result)
{
  init_decimal(result);
  int mem = 0;
  for (int i = 0; i < 96; i++)
  {
    int dec1_bit = get_bit(dec1, i);
    int dec2_bit = get_bit(dec2, i);

    set_bit(result, i, dec1_bit ^ dec2_bit ^ mem);
    mem = (dec1_bit && dec2_bit) || (dec1_bit && mem) || (dec2_bit && mem);
  }
  return mem ? INF : OK;
}