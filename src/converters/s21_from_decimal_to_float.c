#include "../s21_decimal.h"


int s21_from_decimal_to_float(s21_decimal src, float *dst){
	if (dst == NULL)
	return CONVERTING_ERROR;
	
	int sign = get_sign(&src);
	int scale = get_scale(&src);
	s21_decimal mantissa = src;
	mantissa.bit[3] = 0;

	if (mantissa.bit[0] == 0 && mantissa.bit[1] == 0 && mantissa.bit[2] == 0 ) {
		if (sign){
		*dst = -0.0f;
	} else {
			*dst = 0.0f;
		}
		return OK;
	}

		while (!(mantissa.bit[2] & 0x80000000) && scale > 0) {
			shift_left(&mantissa);
			scale--;
		}
		double res = 0.0;
		double res = (double)mantissa.bit[0] +
					 (double)mantissa.bit[1] * 4294967296.0 +
					 (double)mantissa.bit[2] * 18446744073709551616.0;

		
		while (scale > 0 ) {
			res /= 10.0;
			scale --;
		}
		if (sign) {
			res = -res;
		}
		*dst = (float)res;
			
			return OK;
		}
	


