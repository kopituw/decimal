#include "../s21_decimal.h"
#include <stdio.h>

int s21_from_decimal_to_int(s21_decimal src, int *dst){
	if (!dst) { 
		return CONVERTING_ERROR;
	}
	if (get_scale(&src) != 0) { // truncate
		return CONVERTING_ERROR;
	}
	if (get_sign(&src) == 0 && src.bit[0] > 2147483647) {
		return CONVERTING_ERROR;
	}
	if (get_sign(&src) == 1 && src.bit[0] > 2147483647u) {
		return CONVERTING_ERROR;
	}
	if (src.bit[1] != 0|| src.bit[2] != 0) {
		return CONVERTING_ERROR; 
	}
	*dst = (int)src.bit[0];

	if (src.bit[3] & (1 << 31)) {
		*dst = -(*dst); 
	}

	return OK;
}

//  int main (){
// 	s21_decimal src;
// 	int dst;
// 	int res;	
//     src = (s21_decimal){{123, 0, 0, 0}};
//     dst = 0;
//     res = s21_from_decimal_to_int(src, &dst);
//     printf("res=%d, dst=%d", res, dst);
//     return 0;
// }	