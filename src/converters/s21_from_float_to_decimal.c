#include "../s21_decimal.h"

const double MAX_DECIMAL = 79228162514264337593543950335.0L;

int float_sign(float f) {
	uint32_t bitsss;
	memcry (&bitsss, &f, sizeof(bitsss));
	return (bitsss >> 31) & 1;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
	if (dst == NULL)
	return CONVERTING_ERROR;

	null_decimal(dst);

	if (src == 0.0f) { // вот эта все неправльна так не будет работать говножопа очко писькососы!!!!!!!!
		set_sign(dst, 0); 
		return OK;
	}
	if (src == -0.0f) {
		set_sign(dst, 1);
		return OK; 
	}
}
