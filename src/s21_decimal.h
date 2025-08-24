#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define S21_DECIMAL_SIGN_MASK (1U << 31)
#define SCALE_MASK 0x00FF0000
#define MAX_DECIMAL 79228162514264337593543950335.0L

// коды возвращаемых результатов
enum returns {
  OK,
  INF,
  NEGATIVE_INF,
  DIVISION_BY_ZERO,
  CONVERSION_ERROR,
  NULL_POINTER_EXCEPTION,
  WRONG_SCALE
};
enum converts
{
    SUCCESS,
	CONVERTING_ERROR
};

typedef struct s21_decimal {
  uint32_t bit[4];
} s21_decimal;

typedef struct {
  uint32_t b_bit[8];
} s21_big_decimal;

// TESTS
int test_s21_is_greater(int *total_tests_count);
int test_s21_bank_round(int *total_tests_count);
int test_s21_is_greater_or_equal(int *total_tests_count);
int test_s21_is_equal(int *total_tests_count);
int test_s21_add(int *total_tests_count);
int test_s21_sub(int *total_tests_count);
int test_s21_div(int *total_tests_count);
int test_s21_mul(int *total_tests_count);
int test_s21_normalize(int *total_tests_count);
int test_s21_round(int *total_tests_count);
int test_s21_floor(int *total_tests_count);

// арифметика

int denya_add_basic(s21_decimal dec1, s21_decimal dec2, s21_decimal *result);
int denya_sub_basic(s21_decimal dec1, s21_decimal dec2, s21_decimal *result);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_big_div(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result);

// сравнение
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal, s21_decimal);

// big decimal comparison functions
int s21_big_is_equal(s21_big_decimal value1, s21_big_decimal value2);
int s21_big_is_not_equal(s21_big_decimal value_1, s21_big_decimal value_2);

// модальное сравнение
int s21_is_less_modal(s21_decimal, s21_decimal);
int s21_is_less_or_equal_modal(s21_decimal, s21_decimal);
int s21_is_greater_modal(s21_decimal, s21_decimal);
int s21_is_greater_or_equal_modal(s21_decimal, s21_decimal);
int s21_is_equal_modal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal_modal(s21_decimal, s21_decimal);

// преобразование
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// другое
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// вспомогательные функции

void init_decimal(s21_decimal *decimal);

void decimal_to_big(s21_decimal value_1, s21_big_decimal *value_2);
int get_scale(const s21_decimal *value);
void set_scale(s21_decimal *value, int scale);
int get_sign(const s21_decimal *value);
void set_sign(s21_decimal *value, int sign);
int get_bit(const s21_decimal value, int bit);
int big_get_bit(const s21_big_decimal value, int bit);
void set_bit(s21_decimal *src, int bit, int value);
void big_set_bit(s21_big_decimal *src, int bit, int value);
int last_bit(s21_decimal numb);
int big_last_bit(s21_big_decimal numb);
int shift_left(s21_decimal *value);
int shift_left_offset(s21_decimal *dec, int offset);
void null_decimal(s21_decimal *decimal);
void big_null_decimal(s21_big_decimal *decimal);
int mul10(s21_decimal *dst);
void big_mul10(s21_big_decimal *dst);
int normalize(s21_decimal *dec1, s21_decimal *dec2);
int big_normalize(s21_decimal value1, s21_decimal value2, s21_big_decimal *big_value1,
                  s21_big_decimal *big_value2, int *scale);
void bank_round(s21_decimal *dec, unsigned count);
// int bank_round(s21_big_decimal *value, s21_big_decimal value2, unsigned iter, unsigned flag);
int is_zero(s21_decimal value);
int big_is_zero(s21_big_decimal value);
int correct_last_bits(s21_decimal value);
int remains(s21_big_decimal c_result, s21_big_decimal value_1,
            s21_big_decimal value_2, s21_big_decimal *result, int scale);
void shift_right(s21_decimal *value, int offset);
void big_shift_right(s21_big_decimal *value, int offset);
int shift_value_len(s21_big_decimal decimal);
int s21_remain(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int div10(const s21_decimal *value, s21_decimal *quotient);
int big_div10(const s21_big_decimal *value, s21_big_decimal *quotient);
void big_bank_round(s21_big_decimal *value, unsigned count);
void remove_zero(s21_decimal *value);

#endif