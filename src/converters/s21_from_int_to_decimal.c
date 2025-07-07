#include "../s21_decimal.h"
#include <stdio.h>

int s21_from_int_to_decimal(int src, s21_decimal *dst){
	if (dst == NULL) {
		return CONVERTING_ERROR;
	}else{
		null_decimal(dst);
	if (src < 0) {
		dst->bit[3] = 1 << 31;
		src = -src;
	}
	dst->bit[0] |= (uint32_t)src;
	return SUCCESS;
}
}

// int main (){
// 	s21_decimal dst;
// 	s21_from_int_to_decimal(120, &dst);
// 	printf("Знак %u\n", (dst.bit[3] >> 31) & 1);
// 	printf("%u\n", dst.bit[0]);
// 	return 0;
// }