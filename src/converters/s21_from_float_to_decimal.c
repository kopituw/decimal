#include "../s21_decimal.h"

const double MAX_DECIMAL = 79228162514264337593543950335.0L;

int float_sign(float f) {
	uint32_t bits_sign;
	memcpy (&bits_sign, &f, sizeof(bits_sign));
	return (bits_sign >> 31) & 1;
}

int float_exponent(float f){
	uint32_t bits_exponent;
	memcpy (&bits_exponent, &f, sizeof(bits_exponent));
	return (bits_exponent >> 23) & 0xFF;
}

int float_mantissa(float f) {
	uint32_t bits_mantissa;
	memcpy (&bits_mantissa, &f, sizeof(bits_mantissa));
	return bits_mantissa & 0x7FFFFF;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
	if (dst == NULL || src != src || src == INFINITY || src == -INFINITY)
		return CONVERTING_ERROR;

	null_decimal(dst);

	if (src == 0.0f) { 
		int float_sign_get = float_sign(src);
		set_sign(dst, float_sign_get);
		return OK;
	}
	
	int sign = float_sign(src);
	int exponent = float_exponent(src);
	int mantissa = float_mantissa(src);

	uint64_t mantissa64 = (uint64_t)mantissa;
	if (exponent != 0) { // для нормализованн чисел нет ведущей едтницы
		mantissa64 |= ((uint64_t)1 << 23);
	} else  if (mantissa64 == 0){
		set_sign(dst, sign);
		return OK;
		// експ == 0 мантисса == 0 то есть число ноль
	} else {
		int real_exponent = -126;
		mantissa64 >>= (-real_exponent);
		if (mantissa64 > MAX_DECIMAL) {
			return CONVERTING_ERROR;
		}
		
	}
}

		
	
