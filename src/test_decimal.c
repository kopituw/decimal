#include <check.h>
#include <stdbool.h>
#include "s21_decimal.h"

typedef struct s21_decimal
{
    uint32_t bit[4];
} s21_decimal;

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
    for (int i; i < 4; i++)
    {
        if (a.bit[i] != b.bit[i])
        {
            return false;
        }
    }
    return true;
}

// Простое сложение
START_TEST(test_simple_add)
{
    s21_decimal a = {{3, 0, 0, 0}};
    s21_decimal b = {{5, 0, 0, 0}};
    s21_decimal result;
    int status = s21_add(a, b, &result);

    ck_assert_int_eq(status, OK);
    ck_assert_uint_eq(result.bit[0], 8);
}
END_TEST

// Добавить кейсы для сложения: с разными знаками (- и -, + и -, - и +)
// С максимальным децималом
// - 0 + 0 = 0
// - 0 + 5 = 5
// с разными экспонентами -1.5(скейл = 1) + 2.5(скейл = 2)
// проверка на удаление нулей в дробной части 200.00 + 300.00 = 500
// тест на возврат кода ошибки нулевой результат и скейл больше 29
