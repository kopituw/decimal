#include "../s21_decimal.h"
#include <stdlib.h>

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) return CONVERTING_ERROR;

  null_decimal(dst);

  dst->bit[3] = (src < 0 ? 1 : 0) << 31;
  dst->bit[0] = abs(src);
  return OK;
}

// int main (){
// 	s21_decimal dst;
// 	s21_from_int_to_decimal("", &dst);
// 	printf("Знак %u\n", (dst.bit[3] >> 31) & 1);
// 	printf("%u\n", dst.bit[0]);
// 	return 0;
// }