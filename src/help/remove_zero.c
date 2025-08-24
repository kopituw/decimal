#include "../s21_decimal.h"

// void remove_zero(s21_decimal *value) {
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

void remove_zero(s21_decimal *value)
{
  s21_decimal c_value = *value;
  int scale = get_scale(&c_value);

  while (scale != 0)
  {
    s21_decimal quotient = {0};
    int remainder = div10(&c_value, &quotient);
    if (remainder == 0)
    {
      c_value = quotient;
      scale--;
    }
    else
    {
      break;
    }
  }

  set_scale(&c_value, scale);
  *value = c_value;
}