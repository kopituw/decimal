#include "../s21_decimal.h"
#include <ctype.h>

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

	if (exponent != 0) { 
		mantissa64 |= ((uint64_t)1 << 23);

	int real_exponent = exponent - 127 -23;

	long double value = (long double)mantissa64;
	if (real_exponent > 0 || real_exponent < 0)
		value *= powl(2.0L, real_exponent);

	if(sign) value = -value;

	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%.28Lf", value);

	char *dot = strchr(buffer, '.');
	int scale = 0;
	if(dot){
		char *end = buffer + strlen(buffer) - 1;
		while (end > dot && *end == '0'){
		*end = '\0';
			-- end;}

		scale = (int)(strlen(dot+1));
		if (scale < 0) scale = 0;
	}
	
	int significant_digits = 0;
	char *p = buffer;
	
	if (*p == '-') p++;

	while (*p == '0') p++;

	while (*p && *p != '.') {
		significant_digits++;
		p++;
	}
	
	if (*p == '.') {
		p++; 
		char *start_after_dot = p;
		while (*p) {
			significant_digits++;
			p++;
		}
		while (p > start_after_dot && *(p-1) == '0') {
			significant_digits--;
			p--;
		}
	}
	
	if (significant_digits > 7) {
		long double mul = powl(10.0L, 7 - significant_digits);
		value = roundl(value * mul) / mul;
		
		snprintf(buffer, sizeof(buffer), "%.28Lf", value);
		
		dot = strchr(buffer, '.');
		scale = 0;
		if(dot){
			char *end = buffer + strlen(buffer) - 1;
			while (end > dot && *end == '0'){
				*end = '\0';
				-- end;
				}
			scale = (int)(strlen(dot+1));
			if (scale < 0) scale = 0;
		}
	}

	char only_digits[64];
	int pos =0 ;
	for (int i =0; buffer[i] != '\0'; i++) {
		if (isdigit((unsigned char)buffer[i])) {
			only_digits[pos++] = buffer[i];
		}
	}
	only_digits[pos] = '\0';
	
	uint64_t integer_part = 0;
	if (strlen(only_digits) > 0) {
		integer_part = strtoull(only_digits, NULL, 10);
	}
	
	if (integer_part > MAX_DECIMAL) {
		return CONVERTING_ERROR;
	}
	
	dst->bit[0] = (uint32_t)(integer_part & 0xFFFFFFFF);
	dst->bit[1] = (uint32_t)((integer_part >> 32) & 0xFFFFFFFF);
	dst->bit[2] = 0; 

	set_scale(dst, scale);
	set_sign(dst, sign);
	
	return OK;
	} else if (mantissa64 == 0) {

		set_sign(dst, sign);
		return OK;
	} else {
		int real_exponent = -126;
		mantissa64 >>= (-real_exponent);
		if (mantissa64 > MAX_DECIMAL) {
			return CONVERTING_ERROR;
		}
		
		dst->bit[0] = (uint32_t)(mantissa64 & 0xFFFFFFFF);
		dst->bit[1] = (uint32_t)((mantissa64 >> 32) & 0xFFFFFFFF);
		dst->bit[2] = 0;
		
		set_scale(dst, 0);
		set_sign(dst, sign);
		
		return OK;
	}
}