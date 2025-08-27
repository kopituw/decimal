#include <check.h>
#include <stdbool.h>

#include "s21_decimal.h"

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
  int i = 0;
  for (; i < 4; i++) {
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
  s21_decimal expected = {{8, 0, 0, 0}};
  s21_decimal result;

  s21_add(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
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

START_TEST(test_simple_add_zeroo_positive) {
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
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL_SCALE(1000000, 3)), 1);
}
END_TEST

START_TEST(test_add_scale_overflow) {
  s21_decimal a = INIT_DECIMAL_SCALE(15, 29);
  s21_decimal b = INIT_DECIMAL_SCALE(275, 2);  // 2.75
  s21_decimal result;

  int status = s21_add(a, b, &result);
  ck_assert_int_eq(status, INF);
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
  ck_assert_int_eq(s21_is_equal(result, INIT_DECIMAL_SCALE(-125, 2)), 1);
}
END_TEST

START_TEST(test_sub_different_scales_2) {
  s21_decimal a = INIT_DECIMAL_SCALE(999999, 3);
  s21_decimal b = INIT_DECIMAL_SCALE(1, 3);
  s21_decimal result;

  s21_sub(a, b, &result);

  s21_decimal expected = INIT_DECIMAL_SCALE(999998, 3);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_scale_overflow) {
  s21_decimal a = INIT_DECIMAL_SCALE(15, 29);
  s21_decimal b = INIT_DECIMAL_SCALE(275, 2);  // 2.75
  s21_decimal result;

  int status = s21_sub(a, b, &result);
  ck_assert_int_eq(status, INF);
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
  s21_decimal b = INIT_DECIMAL(2);
  s21_decimal result;
  int status = s21_mul(max, b, &result);

  ck_assert_int_eq(status, INF);
}
END_TEST

START_TEST(test_mul_overflow_negative) {
  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal b = INIT_DECIMAL(-2);
  s21_decimal result;

  int status = s21_mul(max, b, &result);
  ck_assert_int_eq(status, INF);
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
  s21_decimal exp = INIT_DECIMAL(5);
  s21_decimal result;

  s21_div(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, exp), 1);
}
END_TEST

START_TEST(test_div_2) {
  s21_decimal a = INIT_DECIMAL(5);
  s21_decimal b = INIT_DECIMAL(2);
  s21_decimal exp = INIT_DECIMAL_SCALE(25, 1);
  s21_decimal result;

  s21_div(a, b, &result);
  ck_assert_int_eq(s21_is_equal(result, exp), 1);
}
END_TEST

START_TEST(test_is_equal_1) {
  s21_decimal a = {{0x0, 0x0, 0x0, 0x0}};
  s21_decimal b = {{0x0, 0x0, 0x0, 0x0}};

  int status = s21_is_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_equal_2) {
  s21_decimal a = {{0x1F018BE6, 0x1, 0x0, 0x40000}};
  s21_decimal b = {{0x1F018BE6, 0x1, 0x0, 0x40000}};  // 481516.2342

  int status = s21_is_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_equal_3) {
  s21_decimal a = {{0xF282E200, 0x95AE8703, 0xE70A1, 0x801B0000}};
  s21_decimal b = {{0xF282E200, 0x95AE8703, 0xE70A1,
                    0x801B0000}};  //-0.17456847232188684569862656

  int status = s21_is_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_equal_4) {
  s21_decimal a = {{0xB2400008, 0x19E0C9BA, 0x21E, 0x80160000}};
  s21_decimal b = {
      {0xB2400008, 0x19E0C9BA, 0x21E, 0x80160000}};  //-1.0000000000000000000008

  int status = s21_is_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_equal_5) {
  s21_decimal a = {{0x6FEC01F2, 0xAB44DF0C, 0x0, 0x80080000}};
  s21_decimal b = {
      {0x6FEC01F2, 0xAB44DF0C, 0x0, 0x80080000}};  // -123412341234.12341234

  int status = s21_is_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_equal_6) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};  // max

  int status = s21_is_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_equal_7) {
  s21_decimal a = {{0x0, 0x0, 0x0, 0x80000000}};  // -0
  s21_decimal b = {{0x0, 0x0, 0x0, 0x0}};         // 0

  int status = s21_is_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_equal_8) {
  s21_decimal a = {{0xA, 0x0, 0x0, 0x0}};       // 10
  s21_decimal b = {{0x65, 0x0, 0x0, 0x10000}};  // 10.1

  int status = s21_is_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_equal_9) {
  s21_decimal a = {
      {0x809F4038, 0xAF56227, 0x0, 0x80050000}};  // -7896452314745.56984
  s21_decimal b = {
      {0x809F4038, 0xAF56227, 0x0, 0x80060000}};  // -789645231474.556984

  int status = s21_is_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_equal_10) {
  s21_decimal a = {
      {0xB2400007, 0x19E0C9BA, 0x21E, 0x80160000}};  //-1.0000000000000000000007
  s21_decimal b = {
      {0xB2400008, 0x19E0C9BA, 0x21E, 0x80160000}};  //-1.0000000000000000000008

  int status = s21_is_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_1) {
  s21_decimal a = {{0xCE0DA241, 0x0, 0x0, 0xC0000}};  // 0.003457000001
  s21_decimal b = {
      {0x93BC012F, 0x6124FEE9, 0x0, 0x80030000}};  //-7000000000000000.303

  int status = s21_is_greater_or_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_2) {
  s21_decimal a = {
      {0x41C29A0E, 0xB9, 0x0, 0x80130000}};  //-0.0000000795672222222
  s21_decimal b = {{0x0, 0x0, 0x0, 0x0}};

  int status = s21_is_greater_or_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_3) {
  s21_decimal a = {{0x0, 0x0, 0x0, 0x0}};         // 0
  s21_decimal b = {{0x0, 0x0, 0x0, 0x80000000}};  // -0

  int status = s21_is_greater_or_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_4) {
  s21_decimal a = {{0xCE0DA241, 0x0, 0x0, 0xC0000}};
  s21_decimal b = {{0xCE0DA241, 0x0, 0x0, 0xC0000}};  // 0.003457000001

  int status = s21_is_greater_or_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_5) {
  s21_decimal a = {{0xB, 0x0, 0x0, 0x20000}};  // 0.11
  s21_decimal b = {{0x1, 0x0, 0x0, 0x10000}};  // 0.1

  int status = s21_is_greater_or_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_6) {
  s21_decimal a = {{0xA, 0x0, 0x0, 0x0}};       // 10
  s21_decimal b = {{0x65, 0x0, 0x0, 0x10000}};  // 10.1

  int status = s21_is_greater_or_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_7) {
  s21_decimal a =
      INIT_DECIMAL_SCALE(856423957346364656, 4);  // 85642395734636.4656
  s21_decimal b =
      INIT_DECIMAL_SCALE(856423957346364657, 4);  // 85642395734636.4657

  int status = s21_is_greater_or_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_8) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};  // min
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};         // max

  int status = s21_is_greater_or_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_9) {
  s21_decimal a = {{0x1708, 0x0, 0x0, 0x80000000}};  //-5896
  s21_decimal b = {{0x1EC4, 0x0, 0x0, 0x80000000}};  //-7876

  int status = s21_is_greater_or_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_greater_1) {
  s21_decimal a = {{0x0, 0x0, 0x0, 0x0}};  // 0
  s21_decimal b = {{0x0, 0x0, 0x0, 0x0}};  // 0

  int status = s21_is_greater(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_greater_2) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};         // max
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};  // min

  int status = s21_is_greater(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_greater_3) {
  s21_decimal a = {
      {0x019C74F5, 0x62CB33F9, 0x1, 0x0}};        // 25565584854.265656565
  s21_decimal b = {{0x3, 0x0, 0x0, 0x80000000}};  // 3

  int status = s21_is_greater(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_greater_4) {
  s21_decimal a = {{0x1708, 0x0, 0x0, 0x80000000}};  //-5896
  s21_decimal b = {{0x1EC4, 0x0, 0x0, 0x80000000}};  //-7876

  int status = s21_is_greater(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_greater_5) {
  s21_decimal a = {{0x80, 0x0, 0x0, 0x0}};  // 10000000
  s21_decimal b = {{0x40, 0x0, 0x0, 0x0}};  // 1000000

  int status = s21_is_greater(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_greater_6) {
  s21_decimal a = {{0xF, 0x0, 0x0, 0x80020000}};  // -0.15
  s21_decimal b = {{0x3C5AFE, 0x0, 0x0, 0x0}};    // 3955454

  int status = s21_is_greater(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_greater_7) {
  s21_decimal a = {
      {0x809F4038, 0xAF56227, 0x0, 0x80050000}};  //-7896452314745.56984
  s21_decimal b = {
      {0x809F4038, 0xAF56227, 0x0, 0x80060000}};  //-789645231474.556984

  int status = s21_is_greater(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_greater_8) {
  s21_decimal a = {{0xA, 0x0, 0x0, 0x0}};       // 10
  s21_decimal b = {{0x65, 0x0, 0x0, 0x10000}};  // 10.1

  int status = s21_is_greater(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_greater_9) {
  s21_decimal a = {{0xB, 0x0, 0x0, 0x20000}};  // 0.11
  s21_decimal b = {{0x1, 0x0, 0x0, 0x10000}};  // 0.1

  int status = s21_is_greater(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_greater_10) {
  s21_decimal a = {{0x0, 0x0, 0x0, 0x0}};         // 0
  s21_decimal b = {{0x0, 0x0, 0x0, 0x80000000}};  // -0

  int status = s21_is_greater(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_less_or_equal_1) {
  s21_decimal a = {{0x0, 0x0, 0x0, 0x0}};  // 0
  s21_decimal b = {{0x0, 0x0, 0x0, 0x0}};  // 0

  int status = s21_is_less_or_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_less_or_equal_2) {
  s21_decimal a = {
      {0x6FEC01F2, 0xAB44DF0C, 0x0, 0x80080000}};  //-123412341234.12341234
  s21_decimal b = {
      {0x6FEC01F2, 0xAB44DF0C, 0x0, 0x80080000}};  //-123412341234.12341234

  int status = s21_is_less_or_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_less_or_equal_3) {
  s21_decimal a = {
      {0x809F4038, 0xAF56227, 0x0, 0x80050000}};  //-7896452314745.56984
  s21_decimal b = {
      {0x809F4038, 0xAF56227, 0x0, 0x80050000}};  //-7896452314745.56984

  int status = s21_is_less_or_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_less_or_equal_4) {
  s21_decimal a = {{0xF, 0x0, 0x0, 0x80020000}};  //-0.15
  s21_decimal b = {{0x3C5AFE, 0x0, 0x0, 0x0}};    // 3955454

  int status = s21_is_less_or_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_less_or_equal_5) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};         // max
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};  // min

  int status = s21_is_less_or_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_less_or_equal_6) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};  // min
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};         // max

  int status = s21_is_less_or_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_less_or_equal_7) {
  s21_decimal a = {
      {0xB2400007, 0x19E0C9BA, 0x21E, 0x80160000}};  //-1.0000000000000000000007
  s21_decimal b = {
      {0xB2400008, 0x19E0C9BA, 0x21E, 0x80160000}};  //-1.0000000000000000000008

  int status = s21_is_less_or_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_less_1) {
  s21_decimal a = {{0x0, 0x0, 0x0, 0x0}};  // 0
  s21_decimal b = {{0x0, 0x0, 0x0, 0x0}};  // 0

  int status = s21_is_less(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_less_2) {
  s21_decimal a = {{0x5, 0x0, 0x0, 0x0}};  // 5
  s21_decimal b = {{0x6, 0x0, 0x0, 0x0}};  // 6

  int status = s21_is_less(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_less_3) {
  s21_decimal a = {{0x5, 0x0, 0x0, 0x0}};  // 5
  s21_decimal b = {{0x5, 0x0, 0x0, 0x0}};  // 5

  int status = s21_is_less(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_less_4) {
  s21_decimal a = {
      {0x3AC3ACF0, 0xBE2A10D, 0x0, 0x40000}};           // 85642395734636.4656
  s21_decimal b = {{0x3AC3ACF1, 0xBE2A10D, 0x0, 0x0}};  // 85642395734636.4657

  int status = s21_is_less(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_less_5) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};  // min
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};         // max

  int status = s21_is_less(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_less_6) {
  s21_decimal a = {{0x809F4038, 0xAF56227, 0x0, 0x80050000}};
  s21_decimal b = {{0x809F4038, 0xAF56227, 0x0, 0x80060000}};

  int status = s21_is_less(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_less_7) {
  s21_decimal a = {{0xF, 0x0, 0x0, 0x80020000}};  //-0.15
  s21_decimal b = {{0x3C5AFE, 0x0, 0x0, 0x0}};    // 3955454

  int status = s21_is_less(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_less_8) {
  s21_decimal a = {{0x1708, 0x0, 0x0, 0x80000000}};  //-5896
  s21_decimal b = {{0x1EC4, 0x0, 0x0, 0x80000000}};  //-7876

  int status = s21_is_less(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_less_9) {
  s21_decimal a = {{0x80, 0x0, 0x0, 0x0}};  // 10000000
  s21_decimal b = {{0x40, 0x0, 0x0, 0x0}};  // 1000000

  int status = s21_is_less(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_less_10) {
  s21_decimal a = {{0x0, 0x0, 0x0, 0x80000000}};  // -0
  s21_decimal b = {{0x0, 0x0, 0x0, 0x0}};         // 0

  int status = s21_is_less(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_less_11) {
  s21_decimal a = {{0xA, 0x0, 0x0, 0x0}};
  s21_decimal b = {{0x65, 0x0, 0x0, 0x10000}};

  int status = s21_is_less(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_not_equal_1) {
  s21_decimal a = {{0x0, 0x0, 0x0, 0x0}};  // 0
  s21_decimal b = {{0x0, 0x0, 0x0, 0x0}};  // 0

  int status = s21_is_not_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_not_equal_2) {
  s21_decimal a = {{0x4C4B601, 0x0, 0x0, 0x80000000}};  //-80000513
  s21_decimal b = {{0x11, 0x0, 0x0, 0x80000000}};       //-17

  int status = s21_is_not_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_not_equal_3) {
  s21_decimal a = {{0x76ADF1, 0x0, 0x0, 0x60000}};  // 7.77777
  s21_decimal b = {{0x019C74F5, 0x62CB33F9, 0x1, 0x0}};

  int status = s21_is_not_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_not_equal_4) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};  // min
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};         // max

  int status = s21_is_not_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_not_equal_5) {
  s21_decimal a = {{0xB2400007, 0x19E0C9BA, 0x21E, 0x80160000}};
  s21_decimal b = {{0xB2400008, 0x19E0C9BA, 0x21E, 0x80160000}};

  int status = s21_is_not_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_not_equal_6) {
  s21_decimal a = {{0xB, 0x0, 0x0, 0x20000}};
  s21_decimal b = {{0x1, 0x0, 0x0, 0x10000}};

  int status = s21_is_not_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_not_equal_7) {
  s21_decimal a = {{0x809F4038, 0xAF56227, 0x0, 0x80050000}};
  s21_decimal b = {{0x809F4038, 0xAF56227, 0x0, 0x80060000}};

  int status = s21_is_not_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_not_equal_8) {
  s21_decimal a = {{0xA, 0x0, 0x0, 0x0}};
  s21_decimal b = {{0x65, 0x0, 0x0, 0x10000}};

  int status = s21_is_not_equal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_is_not_equal_9) {
  s21_decimal a = {{0x0, 0x0, 0x0, 0x80000000}};
  s21_decimal b = {{0x0, 0x0, 0x0, 0x0}};

  int status = s21_is_not_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_not_equal_10) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  int status = s21_is_not_equal(a, b);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_is_less_or_equal_modal) {
  s21_decimal a = INIT_DECIMAL(4);
  s21_decimal b = INIT_DECIMAL(5);

  int status = s21_is_less_or_equal_modal(a, b);

  ck_assert_int_eq(status, 1);
}

START_TEST(test_is_not_equal_modal) {
  s21_decimal a = INIT_DECIMAL(4);
  s21_decimal b = INIT_DECIMAL(5);

  int status = s21_is_not_equal_modal(a, b);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_decimal_to_float_1) {
  s21_decimal value = {{0xF, 0x0, 0x0, 0x10000}};
  float res = 0.0f;
  float expected = 1.5f;

  s21_from_decimal_to_float(value, &res);
  ck_assert_float_eq_tol(res, expected, 1e-6);
}
END_TEST

START_TEST(test_decimal_to_float_2) {
  s21_decimal value = INIT_DECIMAL_SCALE(1, 29);
  float res = 0.0f;
  int status;

  status = s21_from_decimal_to_float(value, &res);
  ck_assert_int_eq(status, CONVERTING_ERROR);
}
END_TEST

START_TEST(test_decimal_to_float_3) {
  s21_decimal value = INIT_DECIMAL_SCALE(376126397, 5);
  float res = 0.0f;
  float expected = 3761.26397;

  s21_from_decimal_to_float(value, &res);
  ck_assert_float_eq_tol(res, expected, 1e-6);
}
END_TEST

START_TEST(test_decimal_to_float_4) {
  s21_decimal value = INIT_DECIMAL_SCALE(12345678912345, 7);
  float res = 0.0f;
  float expected = 1234568.0f;

  s21_from_decimal_to_float(value, &res);
  ck_assert_float_eq(res, expected);
}
END_TEST

START_TEST(test_from_decimal_to_float_null) {
  s21_decimal src = {{123, 0, 0, 0}};
  ck_assert_int_eq(s21_from_decimal_to_float(src, NULL), CONVERTING_ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_float_negative_zero) {
  s21_decimal src = {{0, 0, 0, 0x80000000}};
  float dst = 1.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(src, &dst), OK);
  ck_assert_float_eq(dst, -0.0f);
}
END_TEST

START_TEST(test_from_decimal_to_int_null) {
  s21_decimal src = {{123, 0, 0, 0}};
  ck_assert_int_eq(s21_from_decimal_to_int(src, NULL), 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_positive) {
  s21_decimal src = {{12345, 0, 0, 0}};
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
  ck_assert_int_eq(dst, 12345);
}
END_TEST

START_TEST(test_from_decimal_to_int_negative) {
  s21_decimal src = {{12345, 0, 0, 0x80000000}};
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
  ck_assert_int_eq(dst, -12345);
}
END_TEST

START_TEST(test_from_decimal_to_int_positive_with_scale) {
  s21_decimal src = {{1234567, 0, 0, 0x20000}};  // 12345.67
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
  ck_assert_int_eq(dst, 12345);
}
END_TEST

START_TEST(test_from_decimal_to_int_negative_with_scale) {
  s21_decimal src = {{1234567, 0, 0, 0x80020000}};  // -12345.67
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
  ck_assert_int_eq(dst, -12345);  // Должно отбросить дробную часть
}
END_TEST

START_TEST(test_from_decimal_to_int_max_int) {
  s21_decimal src = {{2147483647, 0, 0, 0}};
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
  ck_assert_int_eq(dst, 2147483647);
}
END_TEST

START_TEST(test_from_decimal_to_int_overflow_positive) {
  s21_decimal src = {{2147483648u, 0, 0, 0}};
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_overflow_negative) {
  s21_decimal src = {{2147483649u, 0, 0, 0x80000000}};  // -2147483649 < MIN_INT
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_nonzero_bit1) {
  s21_decimal src = {{0, 1, 0, 0}};  // 4294967296 > MAX_INT
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_nonzero_bit2) {
  s21_decimal src = {{0, 0, 1, 0}};  // Очень большое число
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_zero) {
  s21_decimal src = {{0, 0, 0, 0}};
  int dst = -1;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
  ck_assert_int_eq(dst, 0);
}
END_TEST

START_TEST(test_from_decimal_to_int_negative_zero) {
  s21_decimal src = {{0, 0, 0, 0x80000000}};
  int dst = -1;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
  ck_assert_int_eq(dst, 0);  // -0 становится 0
}
END_TEST

START_TEST(test_from_decimal_to_int_truncate_error) {
  s21_decimal src = {{123, 0, 0, 0x1F0000}};  // scale=31
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 1);
}
END_TEST

START_TEST(test_decimal_to_int_1) {
  s21_decimal value = {{2, 0, 0, 0}};
  int res;
  int expected = 2;

  s21_from_decimal_to_int(value, &res);
  ck_assert_int_eq(res, expected);
}
END_TEST

START_TEST(test_decimal_to_int_2) {
  s21_decimal src = {{5, 0, 0, 0}};
  int dst = 0;
  int exp = 5;

  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
  ck_assert_int_eq(dst, exp);
}
END_TEST

START_TEST(test_decimal_to_int_3) {
  s21_decimal src = {{0, 0, 0, 0}};
  int dst = 123;
  int exp = 0;

  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
  ck_assert_int_eq(dst, exp);
}
END_TEST

START_TEST(test_decimal_to_int_4) {
  s21_decimal src = {{9, 0, 0, 0x00030000}};
  int dst = 1;
  int exp = 0;

  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
  ck_assert_int_eq(dst, exp);
}
END_TEST

START_TEST(test_decimal_to_int_5) {
  s21_decimal src = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_from_decimal_to_int(src, NULL), 1);
}
END_TEST

START_TEST(test_decimal_to_int_6) {
  s21_decimal src = {{2147483648u, 0, 0, 0}};
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 1);
}
END_TEST

START_TEST(test_int_to_decimal_1) {
  int value = 2;
  s21_decimal res = {{0, 0, 0, 0}};
  s21_decimal expected = {{2, 0, 0, 0}};

  s21_from_int_to_decimal(value, &res);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_float_to_decimal_1) {
  float src = 0.0;
  s21_decimal dst = {{0}};
  s21_decimal expected = {{0}};
  ck_assert_int_eq(s21_from_float_to_decimal(src, &dst), 0);
  ck_assert_int_eq(dst.bit[0], expected.bit[0]);
  ck_assert_int_eq(dst.bit[1], expected.bit[1]);
  ck_assert_int_eq(dst.bit[2], expected.bit[2]);
  ck_assert_int_eq(dst.bit[3], expected.bit[3]);
}
END_TEST

START_TEST(test_float_to_decimal_2) {
  float src = 1e-28;
  s21_decimal dst = {{0}};
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_float_to_decimal(src, &dst), 0);
  ck_assert_int_eq(dst.bit[0], expected.bit[0]);
  ck_assert_int_eq(dst.bit[1], expected.bit[1]);
  ck_assert_int_eq(dst.bit[2], expected.bit[2]);
  ck_assert_int_eq(dst.bit[3], expected.bit[3]);
}
END_TEST

START_TEST(test_float_to_decimal_3) {
  float src = 1e-29;
  s21_decimal dst = {{0}};
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_float_to_decimal(src, &dst), 0);
  ck_assert_int_eq(dst.bit[0], expected.bit[0]);
  ck_assert_int_eq(dst.bit[1], expected.bit[1]);
  ck_assert_int_eq(dst.bit[2], expected.bit[2]);
  ck_assert_int_eq(dst.bit[3], expected.bit[3]);
}
END_TEST

START_TEST(test_float_to_decimal_4) {
  float src = -1e-29;
  s21_decimal dst = {{0}};
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_float_to_decimal(src, &dst), 0);
  ck_assert_int_eq(dst.bit[0], expected.bit[0]);
  ck_assert_int_eq(dst.bit[1], expected.bit[1]);
  ck_assert_int_eq(dst.bit[2], expected.bit[2]);
  ck_assert_int_eq(dst.bit[3], expected.bit[3]);
}
END_TEST

START_TEST(test_float_to_decimal_5) {
  float value = -9;
  s21_decimal res = {{0, 0, 0, 0}};
  s21_decimal expected = {{0x9, 0x0, 0, 0x80000000}};

  s21_from_float_to_decimal(value, &res);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_float_to_decimal_6) {
  float src = 1.234567e-21;
  s21_decimal dst = {{0}};
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_float_to_decimal(src, &dst), 0);
  ck_assert_int_eq(dst.bit[0], expected.bit[0]);
  ck_assert_int_eq(dst.bit[1], expected.bit[1]);
  ck_assert_int_eq(dst.bit[2], expected.bit[2]);
  ck_assert_int_eq(dst.bit[3], expected.bit[3]);
}
END_TEST

START_TEST(test_float_to_decimal_7) {
  float value = 1237.0000000777777;
  s21_decimal res = {{0, 0, 0, 0}};
  s21_decimal expected = {{0x4D5, 0x0, 0x0, 0x0}};

  s21_from_float_to_decimal(value, &res);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_float_to_decimal_8) {
  float value = 1.000000000789;
  s21_decimal res = {{0, 0, 0, 0}};
  s21_decimal expected = {{0x1, 0x0, 0x0, 0x0}};

  s21_from_float_to_decimal(value, &res);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_1) {
  s21_decimal src = {{0x1F018BE6, 0x1, 0x0, 0x40000}};  // 123456789.1234
  s21_decimal expected = {{0x758EC, 0x0, 0x0, 0x0}};    // 123456789

  s21_decimal res = {{0}};
  ck_assert_int_eq(s21_truncate(src, &res), 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_2) {
  // -0.14569721534856 (scale=14)
  s21_decimal src = {{0x47139988, 0xD40, 0x0, 0x800E0000}};
  s21_decimal expected = {{0x0, 0x0, 0x0, 0x0}};  // 0

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_3) {
  // 1234379 (целое число)
  s21_decimal src = {{0x12D4CB, 0x0, 0x0, 0x0}};
  s21_decimal expected = {{0x12D4CB, 0x0, 0x0, 0x0}};

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_4) {
  // -7896452314745.56984 (scale=5)
  s21_decimal src = {{0x809F4038, 0xAF56227, 0x0, 0x80050000}};
  s21_decimal expected = {
      {0x893C3679, 0x72E, 0x0, 0x80000000}};  // -7896452314745

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_5) {
  // 85642395734636.4656 (scale=4)
  s21_decimal src = {{0x3AC3ACF0, 0xBE2A10D, 0x0, 0x40000}};
  s21_decimal expected = {{0x2C93526C, 0x4DE4, 0x0, 0x0}};  // 85642395734636

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_6) {
  // -0.0000000795672222222 (scale=19)
  s21_decimal src = {{0x41C29A0E, 0xB9, 0x0, 0x80130000}};
  s21_decimal expected = {{0x0, 0x0, 0x0, 0x0}};  // 0

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_7) {
  // -123412341234.12341234 (scale=8)
  s21_decimal src = {{0x6FEC01F2, 0xAB44DF0C, 0x0, 0x80080000}};
  s21_decimal expected = {
      {0xBBF2E1F2, 0x1C, 0x0, 0x80000000}};  // -123412341234

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_8) {
  // 792281625142643375935439.50335 (scale=5)
  s21_decimal src = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x50000}};
  s21_decimal expected = {
      {0x84230FCF, 0xAC471B47, 0xA7C5, 0x0}};  // 792281625142643375935439

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_9) {
  // -1.0000000000000000000008 (scale=22)
  s21_decimal src = {{0xB2400008, 0x19E0C9BA, 0x21E, 0x80160000}};
  s21_decimal expected = {{0x1, 0x0, 0x0, 0x80000000}};  // -1

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_10) {
  // NULL указатель
  s21_decimal src = {{123, 0, 0, 0}};
  int status = s21_truncate(src, NULL);
  ck_assert_int_eq(status, 5);
}
END_TEST

START_TEST(test_truncate_11) {
  // Минимальное значение
  s21_decimal src = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal expected = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_12) {
  // 7.77777 (scale=5)
  s21_decimal src = {{0x76ADF1, 0x0, 0x0, 0x60000}};
  s21_decimal expected = {{0x7, 0x0, 0x0, 0x0}};  // 7

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_13) {
  // 1237.0000000777777 (scale=13)
  s21_decimal src = {{0x0FEAFE31, 0x2BF273, 0x0, 0xD0000}};
  s21_decimal expected = {{0x4D5, 0x0, 0x0, 0x0}};  // 1237

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_14) {
  // 35309188505043.582404850638318 (scale=15)
  s21_decimal src = {{0x1E585DEE, 0x4B1DBED3, 0x72171380, 0xF0000}};
  s21_decimal expected = {{0xFA35DD3, 0x201D, 0x0, 0x0}};  // 35309188505043

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_truncate_15) {
  // 10.1 (scale=1)
  s21_decimal src = {{0x65, 0x0, 0x0, 0x10000}};
  s21_decimal expected = {{0xA, 0x0, 0x0, 0x0}};  // 10

  s21_decimal res = {{0}};
  int status = s21_truncate(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_floor_1) {
  s21_decimal src = INIT_DECIMAL_SCALE(4815162342, 4);
  s21_decimal expected = INIT_DECIMAL(481516);

  s21_decimal res = {{0}};
  int status = s21_floor(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_floor_2) {
  s21_decimal src = INIT_DECIMAL_SCALE(-789645231474556984, 5);
  s21_decimal expected = INIT_DECIMAL(-7896452314746);

  s21_decimal res = {{0}};
  int status = s21_floor(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_floor_3) {
  s21_decimal src = INIT_DECIMAL_SCALE(-5000016, 6);
  s21_decimal expected = INIT_DECIMAL(-6);

  s21_decimal res = {{0}};
  int status = s21_floor(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_floor_4) {
  s21_decimal src = INIT_DECIMAL_SCALE(6, 0);
  s21_decimal expected = INIT_DECIMAL(6);

  s21_decimal res = {{0}};
  int status = s21_floor(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_negate) {
  s21_decimal val = {{123, 0, 0, 0}};
  s21_decimal expected = {{123, 0, 0, (1 << 31)}};
  s21_decimal result = {0};
  s21_negate(val, &result);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}

START_TEST(test_round_1) {
  s21_decimal src = INIT_DECIMAL_SCALE(4815162342, 4);
  s21_decimal expected = INIT_DECIMAL(481516);

  s21_decimal res = {{0}};
  int status = s21_round(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_round_2) {
  s21_decimal src = INIT_DECIMAL_SCALE(1234567, 0);
  s21_decimal expected = INIT_DECIMAL(1234567);

  s21_decimal res = {{0}};
  int status = s21_round(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_round_3) {
  s21_decimal src = INIT_DECIMAL_SCALE(856423957346364656, 4);
  s21_decimal expected = INIT_DECIMAL(85642395734636);

  s21_decimal res = {{0}};
  int status = s21_round(src, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

// Тесты для div10
START_TEST(test_div10_basic) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  s21_decimal quotient;
  int remainder = div10(&value, &quotient);
  ck_assert_int_eq(remainder, 9);
  ck_assert_uint_eq(quotient.bit[0], 12345678);
}
END_TEST

START_TEST(test_div10_with_remainder) {
  s21_decimal value = {{10, 0, 0, 0}};
  s21_decimal quotient;
  int remainder = div10(&value, &quotient);
  ck_assert_int_eq(remainder, 0);
  ck_assert_uint_eq(quotient.bit[0], 1);
}
END_TEST

START_TEST(test_big_div10_basic) {
  s21_big_decimal value = {{123456789, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal quotient;
  int remainder = big_div10(&value, &quotient);
  ck_assert_int_eq(remainder, 9);
  ck_assert_uint_eq(quotient.b_bit[0], 12345678);
}
END_TEST

START_TEST(test_bank_round_positive) {
  s21_decimal dec = {{123456789, 0, 0, 0}};
  set_scale(&dec, 5);
  bank_round(&dec, 3);
  s21_decimal expected = {{123457, 0, 0, 0}};
  set_scale(&expected, 2);
  ck_assert_int_eq(s21_is_equal(dec, expected), 1);
}
END_TEST

START_TEST(test_bank_round_negative) {
  s21_decimal dec = {{123456789, 0, 0, 0x80000000}};
  set_scale(&dec, 5);
  bank_round(&dec, 3);
  s21_decimal expected = {{123457, 0, 0, 0x80000000}};
  set_scale(&expected, 2);
  ck_assert_int_eq(s21_is_equal(dec, expected), 1);
}
END_TEST

Suite *decimal_suite(void) {
  Suite *s = suite_create("Decimal");
  TCase *tc = tcase_create("Core");
  tcase_add_test(tc, test_simple_add);
  tcase_add_test(tc, test_simple_add_negative_positive);
  tcase_add_test(tc, test_simple_add_positive_negative);
  tcase_add_test(tc, test_simple_add_negative_negative);
  tcase_add_test(tc, test_simple_add_zero_positive);
  tcase_add_test(tc, test_simple_add_zeroo_positive);
  tcase_add_test(tc, test_simple_add_zero_negative);
  tcase_add_test(tc, test_add_max_decimal);
  tcase_add_test(tc, test_add_max_decimal_negative);
  tcase_add_test(tc, test_add_different_scales);
  tcase_add_test(tc, test_add_different_scales_2);
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
  tcase_add_test(tc, test_div_by_zero);
  tcase_add_test(tc, test_div_simple);
  tcase_add_test(tc, test_div_2);
  tcase_add_test(tc, test_is_equal_1);
  tcase_add_test(tc, test_is_equal_2);
  tcase_add_test(tc, test_is_equal_3);
  tcase_add_test(tc, test_is_equal_4);
  tcase_add_test(tc, test_is_equal_5);
  tcase_add_test(tc, test_is_equal_6);
  tcase_add_test(tc, test_is_equal_7);
  tcase_add_test(tc, test_is_equal_8);
  tcase_add_test(tc, test_is_equal_9);
  tcase_add_test(tc, test_is_equal_10);
  tcase_add_test(tc, test_is_greater_or_equal_1);
  tcase_add_test(tc, test_is_greater_or_equal_2);
  tcase_add_test(tc, test_is_greater_or_equal_3);
  tcase_add_test(tc, test_is_greater_or_equal_4);
  tcase_add_test(tc, test_is_greater_or_equal_5);
  tcase_add_test(tc, test_is_greater_or_equal_6);
  tcase_add_test(tc, test_is_greater_or_equal_7);
  tcase_add_test(tc, test_is_greater_or_equal_8);
  tcase_add_test(tc, test_is_greater_or_equal_9);
  tcase_add_test(tc, test_is_greater_1);
  tcase_add_test(tc, test_is_greater_2);
  tcase_add_test(tc, test_is_greater_3);
  tcase_add_test(tc, test_is_greater_4);
  tcase_add_test(tc, test_is_greater_5);
  tcase_add_test(tc, test_is_greater_6);
  tcase_add_test(tc, test_is_greater_7);
  tcase_add_test(tc, test_is_greater_8);
  tcase_add_test(tc, test_is_greater_9);
  tcase_add_test(tc, test_is_greater_10);
  tcase_add_test(tc, test_is_less_or_equal_1);
  tcase_add_test(tc, test_is_less_or_equal_2);
  tcase_add_test(tc, test_is_less_or_equal_3);
  tcase_add_test(tc, test_is_less_or_equal_4);
  tcase_add_test(tc, test_is_less_or_equal_5);
  tcase_add_test(tc, test_is_less_or_equal_6);
  tcase_add_test(tc, test_is_less_or_equal_7);
  tcase_add_test(tc, test_is_less_1);
  tcase_add_test(tc, test_is_less_2);
  tcase_add_test(tc, test_is_less_3);
  tcase_add_test(tc, test_is_less_4);
  tcase_add_test(tc, test_is_less_5);
  tcase_add_test(tc, test_is_less_6);
  tcase_add_test(tc, test_is_less_7);
  tcase_add_test(tc, test_is_less_8);
  tcase_add_test(tc, test_is_less_9);
  tcase_add_test(tc, test_is_less_10);
  tcase_add_test(tc, test_is_less_11);
  tcase_add_test(tc, test_is_not_equal_1);
  tcase_add_test(tc, test_is_not_equal_2);
  tcase_add_test(tc, test_is_not_equal_3);
  tcase_add_test(tc, test_is_not_equal_4);
  tcase_add_test(tc, test_is_not_equal_5);
  tcase_add_test(tc, test_is_not_equal_6);
  tcase_add_test(tc, test_is_not_equal_7);
  tcase_add_test(tc, test_is_not_equal_8);
  tcase_add_test(tc, test_is_not_equal_9);
  tcase_add_test(tc, test_is_not_equal_10);
  tcase_add_test(tc, test_is_not_equal_modal);
  tcase_add_test(tc, test_is_less_or_equal_modal);
  tcase_add_test(tc, test_decimal_to_float_1);
  tcase_add_test(tc, test_decimal_to_float_2);
  tcase_add_test(tc, test_decimal_to_float_3);
  tcase_add_test(tc, test_decimal_to_float_4);
  tcase_add_test(tc, test_from_decimal_to_float_null);
  tcase_add_test(tc, test_from_decimal_to_float_negative_zero);
  tcase_add_test(tc, test_decimal_to_int_1);
  tcase_add_test(tc, test_decimal_to_int_2);
  tcase_add_test(tc, test_decimal_to_int_3);
  tcase_add_test(tc, test_decimal_to_int_4);
  tcase_add_test(tc, test_decimal_to_int_5);
  tcase_add_test(tc, test_decimal_to_int_6);
  tcase_add_test(tc, test_from_decimal_to_int_null);
  tcase_add_test(tc, test_from_decimal_to_int_positive);
  tcase_add_test(tc, test_from_decimal_to_int_negative);
  tcase_add_test(tc, test_from_decimal_to_int_positive_with_scale);
  tcase_add_test(tc, test_from_decimal_to_int_negative_with_scale);
  tcase_add_test(tc, test_from_decimal_to_int_max_int);
  tcase_add_test(tc, test_from_decimal_to_int_overflow_positive);
  tcase_add_test(tc, test_from_decimal_to_int_overflow_negative);
  tcase_add_test(tc, test_from_decimal_to_int_nonzero_bit1);
  tcase_add_test(tc, test_from_decimal_to_int_nonzero_bit2);
  tcase_add_test(tc, test_from_decimal_to_int_zero);
  tcase_add_test(tc, test_from_decimal_to_int_negative_zero);
  tcase_add_test(tc, test_int_to_decimal_1);
  tcase_add_test(tc, test_float_to_decimal_1);
  tcase_add_test(tc, test_float_to_decimal_2);
  tcase_add_test(tc, test_float_to_decimal_3);
  tcase_add_test(tc, test_float_to_decimal_4);
  tcase_add_test(tc, test_float_to_decimal_5);
  tcase_add_test(tc, test_float_to_decimal_6);
  tcase_add_test(tc, test_float_to_decimal_7);
  tcase_add_test(tc, test_float_to_decimal_8);
  tcase_add_test(tc, test_truncate_1);
  tcase_add_test(tc, test_truncate_2);
  tcase_add_test(tc, test_truncate_3);
  tcase_add_test(tc, test_truncate_4);
  tcase_add_test(tc, test_truncate_5);
  tcase_add_test(tc, test_truncate_6);
  tcase_add_test(tc, test_truncate_7);
  tcase_add_test(tc, test_truncate_8);
  tcase_add_test(tc, test_truncate_9);
  tcase_add_test(tc, test_truncate_10);
  tcase_add_test(tc, test_truncate_11);
  tcase_add_test(tc, test_truncate_12);
  tcase_add_test(tc, test_truncate_13);
  tcase_add_test(tc, test_truncate_14);
  tcase_add_test(tc, test_truncate_15);
  tcase_add_test(tc, test_floor_1);
  tcase_add_test(tc, test_floor_2);
  tcase_add_test(tc, test_floor_3);
  tcase_add_test(tc, test_floor_4);
  tcase_add_test(tc, test_negate);
  tcase_add_test(tc, test_round_1);
  tcase_add_test(tc, test_round_2);
  tcase_add_test(tc, test_round_3);
  tcase_add_test(tc, test_div10_basic);
  tcase_add_test(tc, test_div10_with_remainder);
  tcase_add_test(tc, test_big_div10_basic);
  tcase_add_test(tc, test_bank_round_positive);
  tcase_add_test(tc, test_bank_round_negative);
  suite_add_tcase(s, tc);
  return s;
}

int main(void) {
  int number_failed;
  Suite *s = decimal_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}