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

void bank_round(s21_decimal *dec, unsigned count)
{
  if (!dec || is_zero(*dec))
    return;
  int sign = get_sign(dec);
  int scale = get_scale(dec);

  for (unsigned i = 0; i < count; i++)
  {
    s21_decimal quotient;
    int remainder = div10(dec, &quotient);

    // Bankers rounding
    if (remainder > 5)
    {
      s21_decimal one = {{1, 0, 0, 0}};
      denya_add_basic(quotient, one, &quotient);
    }
    else if (remainder == 5)
    {
      if (quotient.bit[0] & 1)
      {
        s21_decimal one = {{1, 0, 0, 0}};
        denya_add_basic(quotient, one, &quotient);
      }
    }
    *dec = quotient;
  }
  set_sign(dec, sign);
  set_scale(dec, scale - count);
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