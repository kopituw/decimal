#include <check.h>
#include <stdbool.h>
#include "s21_decimal.h"

typedef struct s21_decimal
{
    uint32_t bit[4];
} s21_decimal;

typedef struct
{
    uint32_t b_bit[8];
} s21_big_decimal;

// функция хелпер, чтобы сразу задавать тестовым числам целые значения
s21_decimal INIT_DECIMAL(int64_t value)
{
    s21_decimal dec = {0};
    int sign = 0;
    if (value < 0)
    {
        sign = 1;
        value = -value;
    }

    dec.bit[0] = (uint32_t)value;
    if (value > 0xFFFFFFFF)
    {
        dec.bit[1] = (uint32_t)(value >> 32);
    }

    if (sign)
    {
        dec.bit[3] |= 0x80000000;
    }
    return dec;
}

// функция хелпер, чтобы сравнивать структуры побитово
bool is_equal(s21_decimal a, s21_decimal b)
{
    for (int i = 0; i < 4; i++)
    {
        if (a.bit[i] != b.bit[i])
        {
            return false;
        }
    }
    return true;
}

// Функция хелпер для создания децимала с экспонентой
s21_decimal INIT_DECIMAL_SCALE(int64_t value, int scale)
{
    s21_decimal dec = INIT_DECIMAL(value);
    set_scale(&dec, scale);
    return dec;
}

s21_big_decimal INIT_BIG_DECIMAL(uint64_t value)
{
    s21_big_decimal dec = {0};
    dec.b_bit[0] = (uint64_t)(value & 0xFFFFFFFF);
    dec.b_bit[1] = (uint64_t)(value >> 32);
    return dec;
}

bool is_equal_big(s21_big_decimal a, s21_big_decimal b)
{
    for (int i; i < 8; i++)
    {
        if (a.b_bit[i] != b.b_bit[i])
        {
            return false;
        }
    }
    return true;
}

void print_big_decimal(s21_big_decimal dec)
{
    printf("Big decimal bits: ");
    for (int i = 7; i >= 0; i--)
    {
        printf("%08X", dec.b_bit[i]);
    }
    printf("\n");
}

START_TEST(test_bank_round_down)
{
    s21_decimal value = {{12344, 0, 0, 0}};
    // s21_decimal c_value = value;
    // s21_big_decimal value = INIT_BIG_DECIMAL(12344);
    // s21_big_decimal c_value = value;
    unsigned iter = 1;
    unsigned flag = 0;
    bank_round(&value, 1);
    s21_decimal expected = {{1234, 0, 0, 0}};
    int res = is_equal((s21_decimal)value, (s21_decimal)expected);

    ck_assert_int_eq(res, 1);
}
END_TEST

// START_TEST(test_bank_round_up)
// {
//     s21_big_decimal value = INIT_BIG_DECIMAL(12346);
//     s21_big_decimal c_value = value;
//     unsigned iter = 1;
//     unsigned flag = 1;

//     s21_big_decimal expected = INIT_BIG_DECIMAL(1235);

//     bank_round(&value, c_value, iter, flag);
//     ck_assert_int_eq(s21_big_is_equal(value, expected), 1);
// }
// END_TEST

// START_TEST(test_bank_round_zero_down)
// {
//     s21_big_decimal value = INIT_BIG_DECIMAL(123450);
//     s21_big_decimal c_value = value;
//     unsigned iter = 1;
//     unsigned flag = 0;

//     s21_big_decimal expected = INIT_BIG_DECIMAL(12345);

//     bank_round(&value, c_value, iter, flag);
//     ck_assert_int_eq(s21_big_is_equal(value, expected), 1);
// }
// END_TEST

START_TEST(test_bank_round_zero_up)
{
    s21_big_decimal value = INIT_BIG_DECIMAL(123455);
    s21_big_decimal c_value = value;
    unsigned iter = 1;
    unsigned flag = 0;

    s21_big_decimal expected = INIT_BIG_DECIMAL(12346);

    bank_round(&value, c_value, iter, flag);
    ck_assert_int_eq(s21_big_is_equal(value, expected), 1);
}
END_TEST

START_TEST(test_bank_round_flag_set)
{
    s21_big_decimal value = INIT_BIG_DECIMAL(12345);
    s21_big_decimal c_value = value;
    unsigned iter = 1;
    unsigned flag = 1;

    s21_big_decimal expected = INIT_BIG_DECIMAL(1235);

    bank_round(&value, c_value, iter, flag);
    ck_assert_int_eq(s21_big_is_equal(value, expected), 1);
}
END_TEST

Suite *bank_round_suite(void)
{
    Suite *s = suite_create("Bank Round");
    TCase *tc = tcase_create("Core");
    tcase_add_test(tc, test_bank_round_down);
    // tcase_add_test(tc, test_bank_round_up);
    // tcase_add_test(tc, test_bank_round_zero_down);
    // tcase_add_test(tc, test_bank_round_zero_up);
    // tcase_add_test(tc, test_bank_round_flag_set);
    suite_add_tcase(s, tc);
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s = bank_round_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? 0 : 1;
}
