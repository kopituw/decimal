#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst)
{
  if (!dst)
    return CONVERTING_ERROR;

  null_decimal(dst);

  dst->bit[3] = (src < 0 ? 1 : 0) << 31;
  dst->bit[0] = abs(src);
  return OK;
}