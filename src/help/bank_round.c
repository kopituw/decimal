#include "../s21_decimal.h"


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
  quotient->bit[3] = value->bit[3];
  return (int)rem;
}

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
  int system_bit = dec->bit[3];
  int exp = get_scale(dec) - count;
  s21_decimal ten = {{10, 0, 0, 0}}, one = {{1, 0, 0, 0}}, two = {{2, 0, 0, 0}}, two_res = {0};
  while (count > 0 && count < 96)
  {
    s21_decimal base = {10, 0, 0, 0}, one = {1, 0, 0, 0}, two = {2, 0, 0, 0}, two_res = {0};
    s21_decimal dec_mod = {0};
    set_scale(dec, 0);
    s21_remain(*dec, ten, &dec_mod);
    s21_div(*dec, ten, dec);
    if (dec_mod.bit[0] > 5)
    {
      denya_add_basic(*dec, one, dec);
    }
    else if (dec_mod.bit[0] == 5)
    {
      s21_div(*dec, two, &two_res);
      if (s21_is_equal(one, two_res))
        denya_add_basic(*dec, one, dec);
    }
    count--;
  }
  dec->bit[3] = system_bit;
  set_scale(dec, exp);
}

// void remove_zeros(s21_decimal *value)
// {
//   s21_decimal ostatok = {{0, 0, 0, 0}};
//   int tmp_scale = get_scale(value);

//   set_scale(value, 0);
//   s21_remain(*value, (s21_decimal){{10, 0, 0, 0}}, &ostatok);
//   for (int i = tmp_scale; is_zero(ostatok) && i; i--)
//   {
//     s21_remain(*value, (s21_decimal){{10, 0, 0, 0}}, &ostatok);
//     s21_div(*value, (s21_decimal){{10, 0, 0, 0}}, value);
//   }
// }