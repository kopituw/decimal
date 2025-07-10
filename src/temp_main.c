#include <stdio.h>
#include <stdint.h>
#include "s21_decimal.h"

void print_bits(uint32_t value);

void print_decimal(s21_decimal dec);

int main(void)
{
    int total_tests_count = 0, failed_tests_count = 0;
    failed_tests_count += test_s21_add(&total_tests_count);
    failed_tests_count += test_s21_sub(&total_tests_count);
    test_s21_is_greater(&total_tests_count);
    test_s21_is_equal(&total_tests_count);
    test_s21_is_greater_or_equal(&total_tests_count);
    failed_tests_count += test_s21_mul(&total_tests_count);
    // failed_tests_count += test_s21_mul_int(&total_tests_count);

    failed_tests_count += test_s21_mul(&total_tests_count);
    failed_tests_count += test_s21_div(&total_tests_count);

    printf("\n\n%.2f%% of %d tests passed\n\n", 100 - ((double)failed_tests_count * 100 / (double)total_tests_count), total_tests_count);

    // int na = -8, nb = 5;
    // s21_decimal a = {{na > 0 ? na : -na, 0, 0, 0}};
    // s21_decimal b = {{nb > 0 ? nb : -nb, 0, 0, 0}};
    // s21_decimal result;
    // init_decimal(&result);
    // set_sign(&a, (na < 0));
    // set_sign(&b, (nb < 0));
    // s21_add(a, b, &result);
    // char sign1 = get_sign(&a) ? '-' : '+';
    // char sign2 = get_sign(&b) ? '-' : '+';
    // char sign3 = get_sign(&result) ? '-' : '+';

    // printf("%c%u + %c%u = %c%u\n", sign1, a.bit[0], sign2, b.bit[0], sign3, result.bit[0]);
    // printf("%+d + %+d = %+d\n", na, nb, na + nb);

    return 0;
}

int test_s21_add(int *total_tests_count)
{
    int failed_tests_count = 0;
    int values_a[10] = {0, -0, 1, -1, 5, -5, 8, -8, 56786, -98765};
    int values_b[10] = {0, -0, 1, -1, 5, -5, 8, -8, 56786, -98765};

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (values_a[i] != values_b[j])
            {
                (*total_tests_count)++;
                int na = values_a[i], nb = values_b[j];
                s21_decimal a = {{na > 0 ? na : -na, 0, 0, 0}};
                s21_decimal b = {{nb > 0 ? nb : -nb, 0, 0, 0}};
                s21_decimal result;
                init_decimal(&result);
                set_sign(&a, (na < 0));
                set_sign(&b, (nb < 0));
                char my_res[512] = {0};
                char exp_res[512] = {0};

                s21_add(a, b, &result);
                char sign1 = get_sign(&a) ? '-' : '+';
                char sign2 = get_sign(&b) ? '-' : '+';
                char sign3 = get_sign(&result) ? '-' : '+';

                sprintf(my_res, "%c%u + %c%u = %c%u\n", sign1, a.bit[0], sign2, b.bit[0], sign3, result.bit[0]);
                sprintf(exp_res, "%+d + %+d = %+d\n", na, nb, na + nb);

                if (!strcmp(my_res, exp_res))
                {
                    printf("TEST #%d PASSED!\n", *total_tests_count);
                }
                else
                {
                    failed_tests_count++;
                    printf("TEST #%d FAILED!\n", *total_tests_count);
                    printf(" my_res: %s\nexp_res: %s\n", my_res, exp_res);
                }
            }
        }
    }
    return failed_tests_count;
}

int test_s21_sub(int *total_tests_count)
{
    int failed_tests_count = 0;
    int values_a[10] = {
        0,
        -0,
        1,
        -1,
        5,
        -5,
        8,
        -8,
    };
    int values_b[10] = {
        0,
        -0,
        1,
        -1,
        5,
        -5,
        8,
        -8,
    };

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (values_a[i] != values_b[j])
            {
                (*total_tests_count)++;
                int na = values_a[i], nb = values_b[j];
                s21_decimal a = {{na > 0 ? na : -na, 0, 0, 0}};
                s21_decimal b = {{nb > 0 ? nb : -nb, 0, 0, 0}};
                s21_decimal result;
                init_decimal(&result);
                set_sign(&a, (na < 0));
                set_sign(&b, (nb < 0));
                char my_res[512] = {0};
                char exp_res[512] = {0};

                s21_sub(a, b, &result);
                char sign1 = get_sign(&a) ? '-' : '+';
                char sign2 = get_sign(&b) ? '-' : '+';
                char sign3 = get_sign(&result) ? '-' : '+';

                sprintf(my_res, "%c%u - %c%u = %c%u\n", sign1, a.bit[0], sign2, b.bit[0], sign3, result.bit[0]);
                sprintf(exp_res, "%+d - %+d = %+d\n", na, nb, na - nb);

                if (!strcmp(my_res, exp_res))
                {
                    printf("TEST #%d PASSED!\n", *total_tests_count);
                    // printf(" my_res: %s\nexp_res: %s\n", my_res, exp_res);
                }
                else
                {
                    failed_tests_count++;
                    printf("TEST #%d FAILED!\n", *total_tests_count);
                    printf(" my_res: %s\nexp_res: %s\n", my_res, exp_res);
                }
            }
        }
    }
    return failed_tests_count;
}

int test_s21_mul(int *total_tests_count)
{
    int failed_tests_count = 0;
    int values_a[10] = {0, -0, 1, -1, 5, -5, 8, -8, 45, 6};
    int values_b[10] = {0, -0, 1, -1, 5, -5, 8, -8, 45, 6};

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (values_a[i] != values_b[j])
            {
                (*total_tests_count)++;
                int na = values_a[i], nb = values_b[j];
                s21_decimal a = {{na > 0 ? na : -na, 0, 0, 0}};
                s21_decimal b = {{nb > 0 ? nb : -nb, 0, 0, 0}};
                s21_decimal result;
                init_decimal(&result);
                set_sign(&a, (na < 0));
                set_sign(&b, (nb < 0));
                char my_res[512] = {0};
                char exp_res[512] = {0};

                s21_mul(a, b, &result);
                char sign1 = get_sign(&a) ? '-' : '+';
                char sign2 = get_sign(&b) ? '-' : '+';
                char sign3 = get_sign(&result) ? '-' : '+';

                sprintf(my_res, "%c%u * %c%u = %c%u\n", sign1, a.bit[0], sign2, b.bit[0], sign3, result.bit[0]);
                sprintf(exp_res, "%+d * %+d = %+d\n", na, nb, na * nb);

                if (!strcmp(my_res, exp_res))
                {
                    // printf("TEST #%d PASSED!\n", *total_tests_count);
                    // printf(" my_res: %s\nexp_res: %s\n", my_res, exp_res);
                }
                else
                {
                    failed_tests_count++;
                    printf("TEST #%d FAILED!\n", *total_tests_count);
                    printf(" my_res: %s\nexp_res: %s\n", my_res, exp_res);
                }
            }
        }
    }
    return failed_tests_count;
}

int test_s21_div(int *total_tests_count)
{
    int failed_tests_count = 0;
    int values_a[10] = {11, 5, 8, 6, 26, 10, 2};
    int values_b[10] = {11, 5, 8, 29, 9, 10, 2};

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (values_a[i] != values_b[j])
            {
                (*total_tests_count)++;
                int na = values_a[i], nb = values_b[j];
                s21_decimal a = {{na > 0 ? na : -na, 0, 0, 0}};
                s21_decimal b = {{nb > 0 ? nb : -nb, 0, 0, 0}};
                s21_decimal result;
                init_decimal(&result);
                set_sign(&a, (na < 0));
                set_sign(&b, (nb < 0));
                char my_res[512] = {0};
                char exp_res[512] = {0};

                s21_div(a, b, &result);
                char sign1 = get_sign(&a) ? '-' : '+';
                char sign2 = get_sign(&b) ? '-' : '+';
                char sign3 = get_sign(&result) ? '-' : '+';

                sprintf(my_res, "%c%u / %c%u = %c%u\n", sign1, a.bit[0], sign2, b.bit[0], sign3, result.bit[0]);
                sprintf(exp_res, "%+d / %+d = %+d\n", na, nb, na / nb);

                if (!strcmp(my_res, exp_res))
                {
                    printf("TEST #%d PASSED!\n", *total_tests_count);
                    // printf(" my_res: %s\nexp_res: %s", my_res, exp_res);
                }
                else
                {
                    failed_tests_count++;
                    printf("TEST #%d FAILED!\n", *total_tests_count);
                    printf(" my_res: %s\nexp_res: %s\n", my_res, exp_res);
                }
            }
        }
    }
    return failed_tests_count;
}

// int test_s21_mul_int(int *total_tests_count)
// {
//     int failed_tests_count = 0;
//     int values_a[10] = {1, -1, 5, -5, 8, -8};
//     int values_b[10] = {1, -1, 5, -5, 8, -8};

//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = 0; j < 10; j++)
//         {
//             if (values_a[i] != values_b[j])
//             {
//                 (*total_tests_count)++;
//                 int na = values_a[i], nb = values_b[j];
//                 s21_decimal a = {{na > 0 ? na : -na, 0, 0, 0}};
//                 s21_decimal result;
//                 init_decimal(&result);
//                 set_sign(&a, (na < 0));
//                 char my_res[512] = {0};
//                 char exp_res[512] = {0};

//                 s21_mul_int(a, nb, &result);
//                 char sign1 = get_sign(&a) ? '-' : '+';
//                 char sign3 = get_sign(&result) ? '-' : '+';

//                 sprintf(my_res, "%c%u * %+d = %c%u\n", sign1, a.bit[0], nb, sign3, result.bit[0]);
//                 sprintf(exp_res, "%+d * %+d = %+d\n", na, nb, na * nb);

//                 if (!strcmp(my_res, exp_res))
//                 {
//                     printf("TEST #%d PASSED!\n", *total_tests_count);
//                 }
//                 else
//                 {
//                     failed_tests_count++;
//                     printf("TEST #%d FAILED!\n", *total_tests_count);
//                     printf(" my_res: %s\nexp_res: %s\n", my_res, exp_res);
//                 }
//             }
//         }
//     }
//     return failed_tests_count;
// }

int test_s21_is_greater(int *total_tests_count)
{
    s21_decimal decs1[6] = {{{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{1, 1, 0, 0}}, {{1, 0, 1, 0}}, {{1, 1, 1, 0}}};
    s21_decimal decs2[6] = {{{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{1, 1, 0, 0}}, {{1, 0, 1, 0}}, {{1, 1, 1, 0}}};
    int failed_tests_count = 0;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            (*total_tests_count)++;

            char res[3] = "=<";
            if (s21_is_greater(decs1[i], decs2[j]))
                strcpy(res, ">");

            printf("Result: %d%d%d %s %d%d%d\n", decs1[i].bit[2], decs1[i].bit[1], decs1[i].bit[0], res, decs2[j].bit[2], decs2[j].bit[1], decs2[j].bit[0]);
        }
    }
    return failed_tests_count;
}

int test_s21_is_greater_or_equal(int *total_tests_count)
{
    s21_decimal decs1[6] = {{{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{1, 1, 0, 0}}, {{1, 0, 1, 0}}, {{1, 1, 1, 0}}};
    s21_decimal decs2[6] = {{{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{1, 1, 0, 0}}, {{1, 0, 1, 0}}, {{1, 1, 1, 0}}};
    int failed_tests_count = 0;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            (*total_tests_count)++;

            char res[3] = "<";
            if (s21_is_greater_or_equal(decs1[i], decs2[j]))
                strcpy(res, ">=");

            printf("Result: %d%d%d %s %d%d%d\n", decs1[i].bit[2], decs1[i].bit[1], decs1[i].bit[0], res, decs2[j].bit[2], decs2[j].bit[1], decs2[j].bit[0]);
        }
    }
    return failed_tests_count;
}

int test_s21_is_equal(int *total_tests_count)
{
    s21_decimal decs1[6] = {{{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{1, 1, 0, 0}}, {{1, 0, 1, 0}}, {{1, 1, 1, 0}}};
    s21_decimal decs2[6] = {{{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{1, 1, 0, 0}}, {{1, 0, 1, 0}}, {{1, 1, 1, 0}}};
    int failed_tests_count = 0;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            (*total_tests_count)++;

            char res[3] = "!=";
            if (s21_is_equal(decs1[i], decs2[j]))
                strcpy(res, "=");

            printf("Result: %d%d%d %s %d%d%d\n", decs1[i].bit[2], decs1[i].bit[1], decs1[i].bit[0], res, decs2[j].bit[2], decs2[j].bit[1], decs2[j].bit[0]);
        }
    }
    return failed_tests_count;
}

void print_bits(uint32_t value)
{
    for (int i = 31; i >= 0; --i)
    {
        printf("%u", (value >> i) & 1);
    }
}

void print_decimal(s21_decimal dec)
{
    // printf("Result num: %d\n", dec.bits[0]);

    printf("Bits:\n");
    for (int i = 0; i < 4; ++i)
    {
        printf("bits[%d]: ", i);
        print_bits(dec.bit[i]);
        printf("\n");
    }

    // Извлекаем scale и знак из bits[3]
    uint32_t bits3 = dec.bit[3];

    // int scale = (bits3 >> 16) & 0xFF;
    int scale = get_scale(&dec);
    // int sign = (bits3 >> 31) & 1;
    int sign = get_sign(&dec);

    printf("Scale (decimal places): %d\n", scale);
    printf("Sign: %s\n", sign ? "negative" : "positive");
}
