#include "../s21_decimal.h"

// Делит value на 10, кладёт результат в quotient, остаток возвращает
int div10(const s21_decimal *value, s21_decimal *quotient)
{
  init_decimal(quotient);
  uint64_t rem = 0;
  for (int i = 2; i >= 0; --i)
  {
    uint64_t acc = ((uint64_t)rem << 32) | value->bit[i];
    quotient->bit[i] = (uint32_t)(acc / 10);
    rem = acc % 10;
  }
  quotient->bit[3] = value->bit[3]; // копируем служебные биты (знак, scale)
  return (int)rem;
}

// Делит value на 10, кладёт результат в quotient, остаток возвращает
int big_div10(const s21_big_decimal *value, s21_big_decimal *quotient)
{
  big_null_decimal(quotient);
  uint64_t rem = 0;
  for (int i = 7; i >= 0; --i)
  {
    uint64_t acc = ((uint64_t)rem << 32) | value->b_bit[i];
    quotient->b_bit[i] = (uint32_t)(acc / 10);
    rem = acc % 10;
  }
  return (int)rem;
}

// void bank_round(s21_decimal *dec, unsigned count)
// {
//   if (dec == NULL)
//     return NULL_POINTER_EXCEPTION;

//   int sign = get_sign(dec);
//   int scale = get_scale(dec);

//   // Если масштаб 0, число уже целое
//   if (scale == 0)
//     return OK;

//   if (count > scale)
//     count = scale;

//   s21_decimal value = *dec;

//   // Работаем с положительным числом
//   set_sign(&value, 0);

//   // Создаем decimal для числа 5 с нужным масштабом (0.5)
//   s21_decimal half = {{5, 0, 0, 0}};
//   set_scale(&half, 1); // 0.5

//   // Умножаем half на 10^(scale-1) чтобы получить 0.5 в нужном масштабе
//   for (int i = 0; i < scale - 1; i++)
//   {
//     mul10(&half);
//   }
//   set_scale(&half, scale);

//   // Получаем целую часть
//   s21_decimal integer_part;
//   s21_truncate(value, &integer_part);

//   // Вычисляем дробную часть
//   s21_decimal fractional;
//   s21_sub(value, integer_part, &fractional);

//   // Сравниваем дробную часть с 0.5
//   if (s21_is_greater(fractional, half))
//   {
//     s21_add(integer_part, (s21_decimal){{1, 0, 0, 0}}, &integer_part);
//   }
//   if (s21_is_equal(fractional, half))
//   {
//     s21_decimal two = {{2, 0, 0, 0}};
//     // s21_decimal half = {{5, 0, 0, 0}};
//     s21_decimal remain = {{0, 0, 0, 0}};
//     s21_div_basic(*dec, (s21_decimal){{2, 0, 0, 0}}, &remain, &remain);
//     if (s21_is_less(remain, two))
//     {
//       s21_add(integer_part, (s21_decimal){{1, 0, 0, 0}}, &integer_part);
//     }
//   }

//   // Восстанавливаем знак
//   set_sign(&integer_part, sign);
//   *dec = integer_part;

//   return OK;
// }

void bank_round(s21_decimal *dec, unsigned count)
{
  if (dec == NULL)
    return NULL_POINTER_EXCEPTION;

  int scale = get_scale(dec);
  // set_scale(dec, 0);
  // printf("scale = %d\n", scale);

  if (scale != 0 && count <= scale)
  {

    int sign = get_sign(dec);
    int scale_res = scale - count;
    s21_decimal ten = {0}, one = {0}, two = {0}, two_res = {0};
    s21_from_int_to_decimal(10, &ten);
    s21_from_int_to_decimal(1, &one);
    s21_from_int_to_decimal(2, &two);
    // Создаем decimal для числа 5 с нужным масштабом (0.5)
    s21_decimal half;
    s21_from_int_to_decimal(5, &half);
    // set_scale(&half, 1); // 0.5

    // Умножаем half на 10^(scale-1) чтобы получить 0.5 в нужном масштабе
    // for (int i = 0; i < scale - 1; i++)
    // {
    //   // s21_mul(half, (s21_decimal){{10, 0, 0, 0}}, &half);
    //   mul10(&half);
    // }
    set_scale(&half, scale_res + 1);

    s21_decimal ipart = {0};
    s21_truncate(*dec, &ipart);

    s21_decimal fpart = {0};
    s21_sub(*dec, ipart, &fpart);
    // printf("fpart = %u scl %d\n", fpart.bit[0], get_scale(&fpart));
    // printf("half = %u scl %d\n", half.bit[0], get_scale(&half));
    // while (count > 0 && count < 96)
    // {
    if (s21_is_greater_modal(fpart, half))
    {
      // printf("fpart %u.%d>5.%d\n", fpart.bit[0], get_scale(&fpart), get_scale(&half));
      set_scale(dec, count);
      s21_truncate(*dec, dec);
      // set_scale(&one, scale);
      denya_add_basic(*dec, one, dec);
    }
    else if (s21_is_equal_modal(fpart, half))
    {
      s21_decimal huy = {0};
      // printf("fpart %u==5\n", fpart.bit[0]);
      set_scale(dec, 0);
      s21_div_basic(*dec, two, &huy, &two_res);
      if (s21_is_equal(one, two_res))
      {
        set_scale(dec, scale);
        s21_truncate(*dec, dec);
        denya_add_basic(*dec, one, dec);
      }
      // }
      // count--;
    }
    set_sign(dec, sign);
    // dec->bit[3] = system_bit;
    set_scale(dec, scale_res);
  }
  // remove_zeros(dec);
}

// void bank_round(s21_decimal *dec, unsigned count)
// {
//   if (!dec || is_zero(*dec))
//     return;
//   int sign = get_sign(dec);
//   int scale = get_scale(dec);

//   for (unsigned i = 0; i < count; i++)
//   {
//     s21_decimal quotient;
//     int remainder = div10(dec, &quotient);

//     // Bankers rounding
//     if (remainder > 5)
//     {
//       s21_decimal one = {{1, 0, 0, 0}};
//       denya_add_basic(quotient, one, &quotient);
//     }
//     else if (remainder == 5)
//     {
//       if (quotient.bit[0] & 1)
//       {
//         s21_decimal one = {{1, 0, 0, 0}};
//         denya_add_basic(quotient, one, &quotient);
//       }
//     }
//     *dec = quotient;
//   }
//   set_sign(dec, sign);
//   set_scale(dec, scale - count);
// }

void remove_zeros(s21_decimal *value)
{
  // printf("========================\nit was: %u.%u.%u\n", value->bit[0], value->bit[1], value->bit[2]);

  // printf("in %u.%u.%u ", value->bit[0], value->bit[1], value->bit[2]);
  // printf("with scale of %d ", get_scale(value));
  s21_decimal ostatok = {{0, 0, 0, 0}};
  int tmp_scale = get_scale(value);

  set_scale(value, 0);
  s21_remain(*value, (s21_decimal){{10, 0, 0, 0}}, &ostatok);
  for (int i = tmp_scale; is_zero(ostatok) && i; i--)
  {
    s21_remain(*value, (s21_decimal){{10, 0, 0, 0}}, &ostatok);
    s21_div(*value, (s21_decimal){{10, 0, 0, 0}}, value);
  }
  // printf("%d zeros deleted\n", temp_counter);
  // printf("zeros removed = %u.%u.%u\n", value->bit[0], value->bit[1], value->bit[2]);
}

// void big_bank_round(s21_big_decimal *value, unsigned count) {
//   if (!value || big_is_zero(*value)) return;
//   for (unsigned i = 0; i < count; i++) {
//     s21_big_decimal quotient = {0};
//     int remainder = big_div10(value, &quotient);
//     // Bankers rounding
//     if (remainder > 5) {
//       s21_big_decimal one = {0};
//       one.b_bit[0] = 1;
//       s21_big_add(quotient, one, &quotient);
//     } else if (remainder == 5) {
//       if (quotient.b_bit[0] & 1) {
//         s21_big_decimal one = {0};
//         one.b_bit[0] = 1;
//         s21_big_add(quotient, one, &quotient);
//       }
//     }
//     *value = quotient;
//   }
// }

// #include "../s21_decimal.h"

// int bank_round(s21_big_decimal *value, s21_big_decimal value2, unsigned iter,
// unsigned flag)
// {
//     s21_big_decimal c_value = *value, one = {0};
//     one.b_bit[0] = 1;
//     unsigned a = 0, b = 0;

//     for (unsigned i = 0; i < iter; i++)
//     {
//         // big_div10(&value, &a);
//     }

//     // b = big_div10_rest(c_value); // дописать функцию господи боже какой
//     треш

//     if (a == 5 && flag > 0)
//         a += 1;

//     if (a == 5)
//     {
//         if (b % 2 != 0)
//         {
//             s21_big_add(c_value, one, &c_value);
//         }
//     }
//     else if (a > 5)
//     {
//         s21_big_add(c_value, one, &c_value);
//     }

//     *value = c_value;

//     return 0;
// }