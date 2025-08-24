#include "../s21_decimal.h"

int normalize(s21_decimal *dec1, s21_decimal *dec2)
{
  int scale1 = get_scale(dec1), scale2 = get_scale(dec2);

  if (scale1 >= 28 || scale2 >= 28)
    return INF;

  if (scale1 < 0 || scale2 < 0)
    return NEGATIVE_INF;

  int min_scale = scale1 > scale2 ? scale2 : scale1,
      max_scale = scale1 > scale2 ? scale1 : scale2;
  s21_decimal *min_dec = min_scale == scale1 ? dec1 : dec2;
  s21_decimal *max_dec = max_scale == scale1 ? dec1 : dec2;

  while (min_scale != max_scale && min_scale < 28)
  {
    if (!s21_mul(*min_dec, (s21_decimal){{10, 0, 0, 0}}, min_dec))
    {
      min_scale++;
      set_scale(min_dec, min_scale);
    }
  }

  set_scale(min_dec, min_scale);
  set_scale(max_dec, min_scale);

  return OK;
}
