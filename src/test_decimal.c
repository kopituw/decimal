#include <check.h>
#include <stdbool.h>

#include "s21_decimal.h"

// функция хелпер, чтобы сразу задавать тестовым числам целые значения
s21_decimal INIT_DECIMAL(int64_t value) {
  s21_decimal dec = {0};
  int sign = 0;
  if (value < 0) {
    sign = 1;
    value = -value;
  }

  dec.bit[0] = (uint32_t)value;
  if (value > 0xFFFFFFFF) {
    dec.bit[1] = (uint32_t)(value >> 32);
  }

  if (sign) {
    dec.bit[3] |= 0x80000000;
  }
  return dec;
}

// функция хелпер, чтобы сравнивать структуры побитово
bool is_equal(s21_decimal a, s21_decimal b) {
  for (int i = 0; i < 4; i++) {
    if (a.bit[i] != b.bit[i]) {
      return false;
    }
  }
  return true;
}

// Функция хелпер для создания децимала с экспонентой
s21_decimal INIT_DECIMAL_SCALE(int64_t value, int scale) {
  s21_decimal dec = INIT_DECIMAL(value);
  set_scale(&dec, scale);
  return dec;
}

// Простое сложение
START_TEST(test_simple_add) {
  s21_decimal a = {{3, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal result;

  s21_add(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(8)), 1);
}
END_TEST

START_TEST(test_simple_add_negative_positive) {
  s21_decimal a = INIT_DECIMAL(-5);
  s21_decimal b = INIT_DECIMAL(3);
  s21_decimal result;

  s21_add(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(-2)), 1);
}
END_TEST

START_TEST(test_simple_add_positive_negative) {
  s21_decimal a = INIT_DECIMAL(5);
  s21_decimal b = INIT_DECIMAL(-3);
  s21_decimal result;

  s21_add(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(2)), 1);
}
END_TEST

START_TEST(test_simple_add_negative_negative) {
  s21_decimal a = INIT_DECIMAL(-5);
  s21_decimal b = INIT_DECIMAL(-3);
  s21_decimal result;

  s21_add(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(-8)), 1);
}
END_TEST

START_TEST(test_simple_add_zero) {
  s21_decimal a = INIT_DECIMAL(0);
  s21_decimal b = INIT_DECIMAL(0);
  s21_decimal result;

  s21_add(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(0)), 1);
}
END_TEST

START_TEST(test_simple_add_zero_negative) {
  s21_decimal a = INIT_DECIMAL(-0);
  s21_decimal b = INIT_DECIMAL(-0);
  s21_decimal result;

  s21_add(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(0)), 1);
}
END_TEST

START_TEST(test_simple_add_zero_positive) {
  s21_decimal a = INIT_DECIMAL(0);
  s21_decimal b = INIT_DECIMAL(5);
  s21_decimal result;

  s21_add(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(5)), 1);
}
END_TEST

START_TEST(test_add_max_decimal) {
  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal one = INIT_DECIMAL(1);
  s21_decimal result;

  int status = s21_add(max, one, &result);
  ck_assert_int_eq(status, INF);
}
END_TEST

START_TEST(test_add_max_decimal_negative) {
  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal one = INIT_DECIMAL(-1);
  s21_decimal result;

  int status = s21_add(max, one, &result);
  ck_assert_int_eq(status, NEGATIVE_INF);
}
END_TEST

START_TEST(test_add_different_scales) {
  s21_decimal a = INIT_DECIMAL_SCALE(15, 1);   // 1.5
  s21_decimal b = INIT_DECIMAL_SCALE(275, 2);  // 2.75
  s21_decimal result;

  s21_add(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL_SCALE(425, 2)), 1);
}
END_TEST

START_TEST(test_add_different_scales_2) {
  s21_decimal a = INIT_DECIMAL_SCALE(999999, 3);  // 999.999
  s21_decimal b = INIT_DECIMAL_SCALE(1, 3);       // 0.001
  s21_decimal result;

  s21_add(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL_SCALE(1000, 3)), 1);
}
END_TEST

START_TEST(test_add_remove_zeros) {
  s21_decimal a = INIT_DECIMAL_SCALE(200000, 3);  // 200.000
  s21_decimal b = INIT_DECIMAL_SCALE(300000, 3);  // 300.000
  s21_decimal result;

  s21_add(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(500)), 1);
}
END_TEST

// Тесты на вычитание

START_TEST(test_sub_simple) {
  s21_decimal a = INIT_DECIMAL(5);
  s21_decimal b = INIT_DECIMAL(3);
  s21_decimal result;

  s21_sub(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(2)), 1);
}
END_TEST

START_TEST(test_sub_simple_negative_positive) {
  s21_decimal a = INIT_DECIMAL(-5);
  s21_decimal b = INIT_DECIMAL(3);
  s21_decimal result;

  s21_sub(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(-8)), 1);
}
END_TEST

START_TEST(test_sub_simple_negative) {
  s21_decimal a = INIT_DECIMAL(-5);
  s21_decimal b = INIT_DECIMAL(-3);
  s21_decimal result;

  s21_sub(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(-2)), 1);
}
END_TEST

START_TEST(test_sub_simple_positive_negative) {
  s21_decimal a = INIT_DECIMAL(5);
  s21_decimal b = INIT_DECIMAL(-3);
  s21_decimal result;

  s21_sub(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(8)), 1);
}
END_TEST

START_TEST(test_sub_simple_zero) {
  s21_decimal a = INIT_DECIMAL(0);
  s21_decimal b = INIT_DECIMAL(0);
  s21_decimal result;

  s21_sub(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(0)), 1);
}
END_TEST

START_TEST(test_sub_result_zero) {
  s21_decimal a = INIT_DECIMAL(5);
  s21_decimal b = INIT_DECIMAL(5);
  s21_decimal result;

  s21_sub(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(0)), 1);
}
END_TEST

START_TEST(test_sub_max_decimal) {
  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal one = INIT_DECIMAL(1);
  s21_decimal result;

  int status = s21_sub(max, one, &result);
  ck_assert_int_eq(status, NEGATIVE_INF);
}
END_TEST

START_TEST(test_sub_different_scales) {
  s21_decimal a = INIT_DECIMAL_SCALE(15, 1);   // 1.5
  s21_decimal b = INIT_DECIMAL_SCALE(275, 2);  // 2.75
  s21_decimal result;

  s21_sub(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL_SCALE(125, 2)), 1);
}
END_TEST

START_TEST(test_sub_different_scales_2) {
  s21_decimal a = INIT_DECIMAL_SCALE(999999, 3);  // 999.999
  s21_decimal b = INIT_DECIMAL_SCALE(1, 3);       // 0.001
  s21_decimal result;

  s21_sub(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL_SCALE(999.998, 3)), 1);
}
END_TEST

START_TEST(test_sub_remove_zeros) {
  s21_decimal a = INIT_DECIMAL_SCALE(200000, 3);  // 200.000
  s21_decimal b = INIT_DECIMAL_SCALE(300000, 3);  // 300.000
  s21_decimal result;

  s21_sub(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(-100)), 1);
}
END_TEST

START_TEST(test_sub_different_scales_negative) {
  s21_decimal a = INIT_DECIMAL_SCALE(15, -1);
  s21_decimal b = INIT_DECIMAL_SCALE(275, 2);  // 2.75
  s21_decimal result;

  int status = s21_sub(a, b, &result);
  ck_assert_int_eq(status, CONVERSION_ERROR);
}
END_TEST

START_TEST(test_sub_scale_overflow) {
  s21_decimal a = INIT_DECIMAL_SCALE(15, 29);
  s21_decimal b = INIT_DECIMAL_SCALE(275, 2);  // 2.75
  s21_decimal result;

  int status = s21_sub(a, b, &result);
  ck_assert_int_eq(status, CONVERSION_ERROR);
}
END_TEST

// Тесты на умножение

START_TEST(test_mul_simple) {
  s21_decimal a = INIT_DECIMAL(5);
  s21_decimal b = INIT_DECIMAL(3);
  s21_decimal result;

  s21_mul(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(15)), 1);
}
END_TEST

START_TEST(test_mul_simple_negative) {
  s21_decimal a = INIT_DECIMAL(-5);
  s21_decimal b = INIT_DECIMAL(3);
  s21_decimal result;

  s21_mul(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(-15)), 1);
}
END_TEST

START_TEST(test_mul_simple_positive_negative) {
  s21_decimal a = INIT_DECIMAL(5);
  s21_decimal b = INIT_DECIMAL(-3);
  s21_decimal result;

  s21_mul(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(-15)), 1);
}
END_TEST

START_TEST(test_mul_simple_negative_negative) {
  s21_decimal a = INIT_DECIMAL(-5);
  s21_decimal b = INIT_DECIMAL(-3);
  s21_decimal result;

  s21_mul(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(15)), 1);
}
END_TEST

START_TEST(test_mul_simple_zero) {
  s21_decimal a = INIT_DECIMAL(0);
  s21_decimal b = INIT_DECIMAL(3);
  s21_decimal result;

  s21_mul(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(0)), 1);
}
END_TEST

START_TEST(test_mul_simple_zero_negative) {
  s21_decimal a = INIT_DECIMAL(0);
  s21_decimal b = INIT_DECIMAL(-3);
  s21_decimal result;

  s21_mul(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(0)), 1);
}
END_TEST

START_TEST(test_mul_overflow) {
  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  ;
  s21_decimal b = INIT_DECIMAL(1);
  s21_decimal result;
  int status = s21_mul(max, b, &result);

  ck_assert_int_eq(status, INF);
}
END_TEST

START_TEST(test_mul_overflow_negative) {
  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal b = INIT_DECIMAL(-1);
  s21_decimal result;

  int status = s21_mul(max, b, &result);
  ck_assert_int_eq(status, NEGATIVE_INF);
}
END_TEST

START_TEST(test_mul_different_scales) {
  s21_decimal a = INIT_DECIMAL_SCALE(15, 1);   // 1.5
  s21_decimal b = INIT_DECIMAL_SCALE(275, 2);  // 2.75
  s21_decimal result;

  s21_mul(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL_SCALE(4125, 3)), 1);
}
END_TEST

START_TEST(test_mul_remove_zeros) {
  s21_decimal a = INIT_DECIMAL_SCALE(200000, 3);  // 200.000
  s21_decimal b = INIT_DECIMAL_SCALE(300000, 3);  // 300.000
  s21_decimal result;

  s21_mul(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(60000000000)), 1);
}
END_TEST

// Тесты на деление

START_TEST(test_div_by_zero) {
  s21_decimal a = INIT_DECIMAL(3);
  s21_decimal b = INIT_DECIMAL(0);
  s21_decimal result;

  int status = s21_div(a, b, &result);
  ck_assert_int_eq(status, DIVISION_BY_ZERO);
}
END_TEST

START_TEST(test_div_simple) {
  s21_decimal a = INIT_DECIMAL(15);
  s21_decimal b = INIT_DECIMAL(3);
  s21_decimal result;

  s21_div(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL(5)), 1);
}
END_TEST

START_TEST(test_decimal_to_float_1) {
  s21_decimal value = {{0xF, 0x0, 0x0, 0x10000}};
  float res;
  float expected = 1.5;

  s21_from_decimal_to_float(value, &res);
  ck_assert_float_eq(res, expected);
}
END_TEST

START_TEST(test_decimal_to_int_1) {
  s21_decimal value = {{2, 0, 0 , 0}};
  int res;
  int expected = 2;

  s21_from_decimal_to_int(value, &res);
  ck_assert_int_eq(res, expected);
}
END_TEST

Suite* decimal_suite(void) {
  Suite* s = suite_create("Decimal");
  TCase* tc = tcase_create("Core");
  tcase_add_test(tc, test_simple_add);
  tcase_add_test(tc, test_simple_add_negative_positive);
  tcase_add_test(tc, test_simple_add_positive_negative);
  tcase_add_test(tc, test_simple_add_negative_negative);
  tcase_add_test(tc, test_simple_add_zero_positive);
  tcase_add_test(tc, test_simple_add_zero);
  tcase_add_test(tc, test_simple_add_zero_negative);
  tcase_add_test(tc, test_add_max_decimal);
  tcase_add_test(tc, test_add_max_decimal_negative);
  tcase_add_test(tc, test_add_different_scales);
  tcase_add_test(tc, test_add_different_scales_2);
  tcase_add_test(tc, test_add_remove_zeros);
  tcase_add_test(tc, test_add_different_scales_negative);
  tcase_add_test(tc, test_add_scale_overflow);
  tcase_add_test(tc, test_sub_simple);
  tcase_add_test(tc, test_sub_simple_negative_positive);
  tcase_add_test(tc, test_sub_simple_negative);
  tcase_add_test(tc, test_sub_simple_positive_negative);
  tcase_add_test(tc, test_sub_simple_zero);
  tcase_add_test(tc, test_sub_result_zero);
  tcase_add_test(tc, test_sub_max_decimal);
  tcase_add_test(tc, test_sub_different_scales);
  tcase_add_test(tc, test_sub_different_scales_2);
  tcase_add_test(tc, test_sub_remove_zeros);
  tcase_add_test(tc, test_sub_different_scales_negative);
  tcase_add_test(tc, test_sub_scale_overflow);
  tcase_add_test(tc, test_mul_simple);
  tcase_add_test(tc, test_mul_simple_negative);
  tcase_add_test(tc, test_mul_simple_positive_negative);
  tcase_add_test(tc, test_mul_simple_negative_negative);
  tcase_add_test(tc, test_mul_simple_zero);
  tcase_add_test(tc, test_mul_simple_zero_negative);
  tcase_add_test(tc, test_mul_overflow);
  tcase_add_test(tc, test_mul_overflow_negative);
  tcase_add_test(tc, test_mul_different_scales);
  tcase_add_test(tc, test_mul_remove_zeros);
  tcase_add_test(tc, test_div_by_zero);
  tcase_add_test(tc, test_div_simple);
  tcase_add_test(tc, test_decimal_to_float_1);
  tcase_add_test(tc, test_decimal_to_int_1);
  suite_add_tcase(s, tc);
  return s;
}

int main(void) {
  int number_failed;
  Suite* s = decimal_suite();
  SRunner* sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}