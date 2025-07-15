#include "../s21_decimal.h"

const double MAX_DECIMAL = 79228162514264337593543950335.0L;


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
	if (scale > 28){
		*dst = 0.0f;
		return CONVERTING_ERROR;
	}

		while (!(mantissa.bit[2] & 0x80000000) && scale > 0) {
			shift_left(&mantissa);
			scale--;
		}
		// long double res = 0.0;
		long double res = (long double)mantissa.bit[0] +
					 (long double)mantissa.bit[1] * 4294967296.0L+
					 (long double)mantissa.bit[2] * 18446744073709551616.0L;

		while (scale > 0 ) {
			res /= 10.0;
			scale --;
		}
		
		if (sign) {
			res = -res;
		}
		*dst = (float)res;
			if (res  < 0.0000000000000000000000000001 ){
				return CONVERTING_ERROR;
			}
			if (res > MAX_DECIMAL) {
				return CONVERTING_ERROR;
			}
			return OK;
		}



