#include <check.h>
#include <stdbool.h>
#include "s21_decimal.h"

typedef struct s21_decimal {
    uint32_t bit[4];
} s21_decimal;

typedef struct {
    uint32_t b_bit[8];
} s21_big_decimal;

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

s21_big_decimal INIT_BIG_DECIMAL(uint64_t value) {
    s21_big_decimal dec = {0};
    dec.b_bit[0] = (uint64_t)(value & 0xFFFFFFFF);
    dec.b_bit[1] = (uint64_t)(value >> 32);
    return dec;
}

bool is_equal_big(s21_big_decimal a, s21_big_decimal b) {
    for (int i; i < 8; i++) {
        if (a.b_bit[i] != b.b_bit[i]) {
            return false;
        }
    }
    return true;
}

}

START_TEST(tset_bank_round_1) {
