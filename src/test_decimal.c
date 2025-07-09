#include <check.h>
#include <stdbool.h>
#include "s21_decimal.h"

typedef struct s21_decimal {
    uint32_t bit[4];
} s21_decimal;

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
    for (int i; i < 4; i++) {
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
    int status = s21_add(a, b, &result);

    ck_assert_int_eq(status, OK);
    ck_assert_uint_eq(result.bit[0], 8);
} 
END_TEST

START_TEST(test_simple_add_negative_positive) {
    s21_decimal a = INIT_DECIMAL(-5);
    s21_decimal b = INIT_DECIMAL(3);
    s21_decimal result;

    s21_add(a, b, &result);
    ck_assert_int_eq(result, INIT_DECIMAL(-2));
}
END_TEST

START_TEST(test_simple_add_positive_negative) {
    s21_decimal a = INIT_DECIMAL(5);
    s21_decimal b = INIT_DECIMAL(-3);
    s21_decimal result;

    s21_add(a, b, &result);
    ck_assert_int_eq(result, INIT_DECIMAL(2));
}
END_TEST

START_TEST(test_simple_add_negative_negative) {
    s21_decimal a = INIT_DECIMAL(-5);
    s21_decimal b = INIT_DECIMAL(-3);
    s21_decimal result;

    s21_add(a, b, &result);
    ck_assert_int_eq(result, INIT_DECIMAL(-8));
}
END_TEST

START_TEST(test_simple_add_zero_positive) {
    s21_decimal a = INIT_DECIMAL(0);
    s21_decimal b = INIT_DECIMAL(0);
    s21_decimal result;

    s21_add(a, b, &result);
    ck_assert_int_eq(result, INIT_DECIMAL(0));
}
END_TEST

START_TEST(test_simple_add_zero_negative) {
    s21_decimal a = INIT_DECIMAL(-0);
    s21_decimal b = INIT_DECIMAL(-0);
    s21_decimal result;

    s21_add(a, b, &result);
    ck_assert_int_eq(result, INIT_DECIMAL(0));
}
END_TEST

START_TEST(test_simple_add_zero_positive) {
    s21_decimal a = INIT_DECIMAL(0);
    s21_decimal b = INIT_DECIMAL(5);
    s21_decimal result;

    s21_add(a, b, &result);
    ck_assert_int_eq(result, INIT_DECIMAL(5));
}
END_TEST

START_TEST(test_add_max_decimal) {
    s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal one = INIT_DECIMAL(1);
    s21_decimal result;

    int status = s21_add(a, b, &result);
    ck_assert_int_eq(status, INF);
}
END_TEST

START_TEST(test_add_max_decimal_negative) {
    s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1}};
    s21_decimal one = INIT_DECIMAL(-1);
    s21_decimal result;

    int status = s21_add(a, b, &result);
    ck_assert_int_eq(status, NEGATIVE_INF);
}
END_TEST

START_TEST(test_add_different_scales) {
    s21_decimal a = INIT_DECIMAL_SCALE(15, 1); // 1.5
    s21_decimal b = INIT_DECIMAL_SCALE(275, 2); // 2.75
    s21_decimal result;

    s21_add(a, b, &result);
    ck_assert_int_eq(result, INIT_DECIMAL_SCALE(425, 2));
}
END_TEST

START_TEST(test_add_different_scales_negative) {
    s21_decimal a = INIT_DECIMAL_SCALE(15, -1);
    s21_decimal b = INIT_DECIMAL_SCALE(275, 2); // 2.75
    s21_decimal result;

    int status s21_add(a, b, &result);
    ck_assert_int_eq(status, )
}

// с разными экспонентами -1.5(скейл = 1) + 2.5(скейл = 2)
// проверка на удаление нулей в дробной части 200.00 + 300.00 = 500
// тест на возврат кода ошибки нулевой результат и скейл больше 29
