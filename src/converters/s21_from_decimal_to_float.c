#include "../s21_decimal.h"

// const double MAX_DECIMAL = 79228162514264337593543950335.0L;


int s21_from_decimal_to_float(s21_decimal src, float *dst){
	if (dst == NULL)
	return CONVERTING_ERROR;
	
	int sign = get_sign(&src);
	int scale = get_scale(&src); 

	if (scale > 28 || scale < 0) {
		*dst = 0.0f;
		return CONVERTING_ERROR;
	}

	s21_decimal mantissa = src;
	mantissa.bit[3] = 0;

	if (mantissa.bit[0] == 0 && mantissa.bit[1] == 0 && mantissa.bit[2] == 0 ) {
		*dst = sign ? -0.0f : 0.0f;
		return OK;
	}

	long double res;
	if (mantissa.bit[2] == 0 && mantissa.bit[1] == 0){
		res = (long double) mantissa.bit[0];
	} else{
 		
		 res =  (long double)mantissa.bit[0] +
				(long double)mantissa.bit[1] * 4294967296.0L+
				(long double)mantissa.bit[2] * 18446744073709551616.0L;
	}

		while (scale > 0 ) {
			res /= 10.0;
			 scale --;
		}
		
		if (sign) {
			res = -res;
		}
			if (fabsl(res)  < 1e-28){
				*dst = 0.0f;
				return CONVERTING_ERROR;
			}
			if (fabsl(res)> MAX_DECIMAL) {
				*dst = 0.0f;
				return CONVERTING_ERROR;
			}
			long double abs_res = fabsl(res);
			if (abs_res != 0.0L) {
				char buf[64];
				snprintf(buf, sizeof(buf), "%.7g", (double)res); //stdio str witn b.max
				double rounded = strtod(buf, NULL);
				if (sign) rounded = -rounded; // from str to double stdlib
				*dst = (float)rounded;
			}

	return OK;
}



