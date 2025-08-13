#include <stdint.h>
#include <stdio.h>

#include "s21_decimal.h"

void print_bits(uint32_t value);

void print_decimal(s21_decimal dec);

int main(void)
{
  int total_tests_count = 0, failed_tests_count = 0;
  // test_s21_is_greater(&total_tests_count);
  // test_s21_is_equal(&total_tests_count);
  // test_s21_is_greater_or_equal(&total_tests_count);
  // failed_tests_count += test_s21_add(&total_tests_count);
  // failed_tests_count += test_s21_sub(&total_tests_count);
  // failed_tests_count += test_s21_mul(&total_tests_count);
  // failed_tests_count += test_s21_div(&total_tests_count);
  // failed_tests_count += test_s21_normalize(&total_tests_count);
  // failed_tests_count += test_s21_bank_round(&total_tests_count);
  failed_tests_count += test_s21_round(&total_tests_count);
  failed_tests_count += test_s21_floor(&total_tests_count);

  // s21_decimal kek = {{1230000, 0, 0, 0}};
  // set_scale(&kek, 2);
  // s21_decimal ten = {{10, 0, 0, 0}};
  // s21_decimal result = {0};
  // s21_remain(kek, (s21_decimal){{10, 0, 0, 0}}, &result);
  // print_decimal(result);
  // printf("res = %u.%u.%u\n", result.bit[0], result.bit[1], result.bit[2]);

  // remove_zeros(&kek);

  // s21_decimal a = {{1234575, 0, 0, 0}};
  // set_sign(&a, 1);
  // set_scale(&a, 2);
  // s21_decimal result = {{0, 0, 0, 0}};

  // s21_floor(a, &result);
  // printf("res = %u.%u.%u with scale %d\n", result.bit[0], result.bit[1], result.bit[2], get_scale(&result));

  printf("\n\n%.2f%% of %d tests passed\n\n",
         100 - ((double)failed_tests_count * 100 / (double)total_tests_count),
         total_tests_count);

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

  // printf("%c%u + %c%u = %c%u\n", sign1, a.bit[0], sign2, b.bit[0], sign3,
  // result.bit[0]); printf("%+d + %+d = %+d\n", na, nb, na + nb);

  return 0;
}

int test_s21_normalize(int *total_tests_count)
{
  int failed_tests_count = 0;
  int values_a[5] = {0, 1, 5, 8, 26};
  int values_b[5] = {0, 1, 5, 8, 26};

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (values_a[i] != values_b[j])
      {
        (*total_tests_count)++;
        int na = values_a[i], nb = values_b[j];
        s21_decimal a = {{1, 0, 0, 0}};
        s21_decimal b = {{1, 0, 0, 0}};
        s21_decimal result;
        init_decimal(&result);
        set_scale(&a, values_a[i]);
        set_scale(&b, values_b[j]);

        // printf("\nscale = %d %d\n", get_scale(&a), get_scale(&b));
        normalize(&a, &b);

        // sprintf(my_res, "a scale %s b scale\n", get_scale(&a) ==
        // get_scale(&b) ? "==" : "!="); sprintf(exp_res, "a scale %s b
        // scale\n", get_scale(&a) == get_scale(&b) ? "==" : "!=");

        if (get_scale(&a) == get_scale(&b))
        {
          printf("TEST #%d PASSED!\n", *total_tests_count);
          // printf(" my_res: '%s'\nexp_res: '%s'\n", my_res, exp_res);
        }
        else
        {
          failed_tests_count++;
          printf("TEST #%d FAILED!\n", *total_tests_count);
          printf(" my_res: %d %d\nexp_res: %d %d\n", get_scale(&a),
                 get_scale(&b), get_scale(&a), get_scale(&b));
        }
      }
    }
  }
  return failed_tests_count;
}

int test_s21_bank_round(int *total_tests_count)
{
  int failed_tests_count = 0;
  s21_decimal decimals_to_round[5] = {{{16644, 0, 0, 0}},
                                      {{12346, 0, 0, 0}},
                                      {{123450, 0, 0, 0}},
                                      {{123455, 0, 0, 0}},
                                      {{12345, 0, 0, 0}}};
  int r_values[4] = {1, 2, 3, 123};

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      (*total_tests_count)++;
      s21_decimal a = decimals_to_round[i];

      bank_round(&a, r_values[j]);
      printf("dec before: %u and after bank rounding for %d: %u\n\n",
             decimals_to_round[i].bit[0], r_values[j], a.bit[0]);
    }
  }
  return failed_tests_count;
}

int test_s21_round(int *total_tests_count)
{
  int failed_tests_count = 0;
  s21_decimal decimals_to_round[5] = {{{44440, 0, 0, 0}},
                                      {{44450, 0, 0, 0}},
                                      {{44449, 0, 0, 0}},
                                      {{44470, 0, 0, 0}},
                                      {{44482, 0, 0, 0}}};
  // int r_values[4] = {1, 2, 3, 123};
  s21_decimal result = {0};

  for (int i = 0; i < 5; i++)
  {
    if (i == 2 || i == 3)
    {
      // printf("minus\n");
      set_sign(&decimals_to_round[i], 1);
    }
    // for (int j = 0; j < 4; j++)
    // {
    (*total_tests_count)++;
    // s21_decimal a = decimals_to_round[i];
    set_scale(&decimals_to_round[i], 2);

    s21_round(decimals_to_round[i], &result);
    printf("dec before: %s%u and after rounding: %u\n\n",
           get_sign(&decimals_to_round[i]) ? "-" : "+", decimals_to_round[i].bit[0], result.bit[0]);
    // }
  }
  return failed_tests_count;
}

int test_s21_floor(int *total_tests_count)
{
  int failed_tests_count = 0;
  s21_decimal decimals_to_round[5] = {{{449, 0, 0, 0}},
                                      {{442, 0, 0, 0}},
                                      {{448, 0, 0, 0}},
                                      {{443, 0, 0, 0}},
                                      {{442, 0, 0, 0}}};
  s21_decimal result = {0};

  for (int i = 0; i < 5; i++)
  {
    if (i == 2 || i == 3)
      set_sign(&decimals_to_round[i], 1);
    (*total_tests_count)++;

    set_scale(&decimals_to_round[i], 1);

    s21_floor(decimals_to_round[i], &result);
    printf("dec before: %s%u and after flooring: %u\n\n",
           get_sign(&decimals_to_round[i]) ? "-" : "+", decimals_to_round[i].bit[0], result.bit[0]);
  }
  return failed_tests_count;
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

        sprintf(my_res, "%c%u + %c%u = %c%u\n", sign1, a.bit[0], sign2,
                b.bit[0], sign3, result.bit[0]);
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

  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal result;

  int status = s21_add(max, one, &result);

  (*total_tests_count)++;
  if (status != INF)
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
    printf(" my_res: %d\nexp_res: %d\n", status, INF);
  }
  else
  {

    printf("TEST #%d PASSED!\n", *total_tests_count);
  }

  set_sign(&max, 1);
  set_sign(&one, 1);
  status = s21_add(max, one, &result);
  (*total_tests_count)++;

  if (status != NEGATIVE_INF)
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
    printf(" my_res: %d\nexp_res: %d\n", status, INF);
  }
  else
  {
    printf("TEST #%d PASSED!\n", *total_tests_count);
  }

  s21_decimal a = {{999999, 0, 0, 0}}; // 999.999
  s21_decimal b = {{1, 0, 0, 0}};      // 0.001
  set_scale(&a, 3);
  set_scale(&b, 3);

  s21_add(a, b, &result);
  b = (s21_decimal){{1000000, 0, 0, 0}};
  set_scale(&b, 3);

  (*total_tests_count)++;
  if (!s21_is_equal(result, b))
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
    // printf(" my_res: %d\nexp_res: %d\n", status, INF);
  }
  else
  {
    printf("TEST #%d PASSED!\n", *total_tests_count);
  }

  // printf("===========\n");
  a = (s21_decimal){{15, 0, 0, 0}};
  set_scale(&a, -1);
  b = (s21_decimal){{275, 0, 0, 0}};
  set_scale(&b, 2);

  // print_decimal(a);
  // printf("scale -1 %d\n", get_scale(&a));

  status = s21_add(a, b, &result);

  if (status != NEGATIVE_INF)
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
    printf(" my_res: %d\nexp_res: %d\n", status, NEGATIVE_INF);
  }
  else
  {
    printf("TEST scale -1 #%d PASSED!\n", *total_tests_count);
  }

  a = (s21_decimal){{15, 0, 0, 0}};
  set_scale(&a, 29);
  b = (s21_decimal){{275, 0, 0, 0}};
  set_scale(&b, 2);

  print_decimal(a);
  // printf("scale -1 %d\n", get_scale(&a));

  status = s21_add(a, b, &result);

  if (status != INF)
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
    printf(" my_res: %d\nexp_res: %d\n", status, INF);
  }
  else
  {
    printf("TEST scale 29 #%d PASSED!\n", *total_tests_count);
  }

  return failed_tests_count;
}

int test_s21_sub(int *total_tests_count)
{
  int failed_tests_count = 0;
  // int values_a[10] = {
  //     0,
  //     -0,
  //     1,
  //     -1,
  //     5,
  //     -5,
  //     8,
  //     -8,
  // };
  // int values_b[10] = {
  //     0,
  //     -0,
  //     1,
  //     -1,
  //     5,
  //     -5,
  //     8,
  //     -8,
  // };

  // for (int i = 0; i < 10; i++)
  // {
  //   for (int j = 0; j < 10; j++)
  //   {
  //     if (values_a[i] != values_b[j])
  //     {
  //       (*total_tests_count)++;
  //       int na = values_a[i], nb = values_b[j];
  //       s21_decimal a = {{na > 0 ? na : -na, 0, 0, 0}};
  //       s21_decimal b = {{nb > 0 ? nb : -nb, 0, 0, 0}};
  //       s21_decimal result;
  //       init_decimal(&result);
  //       set_sign(&a, (na < 0));
  //       set_sign(&b, (nb < 0));
  //       char my_res[512] = {0};
  //       char exp_res[512] = {0};

  //       s21_sub(a, b, &result);
  //       char sign1 = get_sign(&a) ? '-' : '+';
  //       char sign2 = get_sign(&b) ? '-' : '+';
  //       char sign3 = get_sign(&result) ? '-' : '+';

  //       sprintf(my_res, "%c%u - %c%u = %c%u\n", sign1, a.bit[0], sign2,
  //               b.bit[0], sign3, result.bit[0]);
  //       sprintf(exp_res, "%+d - %+d = %+d\n", na, nb, na - nb);

  //       if (!strcmp(my_res, exp_res))
  //       {
  //         printf("TEST #%d PASSED!\n", *total_tests_count);
  //         // printf(" my_res: %s\nexp_res: %s\n", my_res, exp_res);
  //       }
  //       else
  //       {
  //         failed_tests_count++;
  //         printf("TEST #%d FAILED!\n", *total_tests_count);
  //         printf(" my_res: %s\nexp_res: %s\n", my_res, exp_res);
  //       }
  //     }
  //   }
  // }

  s21_decimal min = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal result;

  int status = s21_sub(min, one, &result);

  (*total_tests_count)++;
  if (status != NEGATIVE_INF)
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
    printf(" my_res: %d\nexp_res: %d\n", status, NEGATIVE_INF);
  }
  else
  {

    printf("TEST #%d PASSED!\n", *total_tests_count);
  }

  s21_decimal a = {{15, 0, 0, 0x00000000}}; // 1.5
  set_scale(&a, 1);
  s21_decimal b = {{275, 0, 0, 0x00000000}}; // 2.75
  set_scale(&b, 2);

  (*total_tests_count)++;
  s21_sub(a, b, &result);

  b = (s21_decimal){{125, 0, 0, b.bit[3]}};
  set_sign(&b, 1);

  if (!s21_is_equal(result, b))
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
  }
  else
  {
    printf("TEST #%d PASSED!\n", *total_tests_count);
  }
  // printf("%s%u with scale of %d\n", get_sign(&result) ? "-" : "", result.bit[0], get_scale(&result));

  a = (s21_decimal){{999999, 0, 0, 0}}; // 999.999
  set_scale(&a, 3);
  b = (s21_decimal){{1, 0, 0, 0}}; // 0.001
  set_scale(&b, 3);

  s21_sub(a, b, &result);

  a = (s21_decimal){{999998, 0, 0, 0}};
  set_scale(&a, 3);

  // print_decimal(a);
  // print_decimal(result);

  if (!s21_is_equal(result, a))
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
  }
  else
  {
    printf("TEST 999998 #%d PASSED!\n", *total_tests_count);
  }

  a = (s21_decimal){{200000, 0, 0, 0}}; // 999.999
  set_scale(&a, 3);
  b = (s21_decimal){{300000, 0, 0, 0}}; // 0.001
  set_scale(&b, 3);

  s21_sub(a, b, &result);

  a = (s21_decimal){{100000, 0, 0, 0}};
  set_scale(&a, 3);

  // printf("%s%u with scale of %d\n", get_sign(&result) ? "-" : "", result.bit[0], get_scale(&result));
  if (s21_is_equal(result, a))
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
  }
  else
  {
    printf("TEST #%d PASSED!\n", *total_tests_count);
  }

  a = (s21_decimal){{15, 0, 0, 0}};
  set_scale(&a, 29);
  b = (s21_decimal){{275, 0, 0, 0}};
  set_scale(&b, 2);

  print_decimal(a);
  // printf("scale -1 %d\n", get_scale(&a));

  status = s21_sub(a, b, &result);

  if (status != INF)
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
    printf(" my_res: %d\nexp_res: %d\n", status, INF);
  }
  else
  {
    printf("TEST scale 29 #%d PASSED!\n", *total_tests_count);
  }

  return failed_tests_count;
}

int test_s21_mul(int *total_tests_count)
{
  int failed_tests_count = 0;
  // int values_a[10] = {0, -0, 1, -1, 5, -5, 8, -8, 45, 6};
  // int values_b[10] = {0, -0, 1, -1, 5, -5, 8, -8, 45, 6};

  // for (int i = 0; i < 10; i++)
  // {
  //   for (int j = 0; j < 10; j++)
  //   {
  //     if (values_a[i] != values_b[j])
  //     {
  //       (*total_tests_count)++;
  //       int na = values_a[i], nb = values_b[j];
  //       s21_decimal a = {{na > 0 ? na : -na, 0, 0, 0}};
  //       s21_decimal b = {{nb > 0 ? nb : -nb, 0, 0, 0}};
  //       s21_decimal result;
  //       init_decimal(&result);
  //       set_sign(&a, (na < 0));
  //       set_sign(&b, (nb < 0));
  //       char my_res[512] = {0};
  //       char exp_res[512] = {0};

  //       s21_mul(a, b, &result);
  //       char sign1 = get_sign(&a) ? '-' : '+';
  //       char sign2 = get_sign(&b) ? '-' : '+';
  //       char sign3 = get_sign(&result) ? '-' : '+';

  //       sprintf(my_res, "%c%u * %c%u = %c%u\n", sign1, a.bit[0], sign2,
  //               b.bit[0], sign3, result.bit[0]);
  //       sprintf(exp_res, "%+d * %+d = %+d\n", na, nb, na * nb);

  //       if (!strcmp(my_res, exp_res))
  //       {
  //         // printf("TEST #%d PASSED!\n", *total_tests_count);
  //         // printf(" my_res: %s\nexp_res: %s\n", my_res, exp_res);
  //       }
  //       else
  //       {
  //         failed_tests_count++;
  //         printf("TEST #%d FAILED!\n", *total_tests_count);
  //         printf(" my_res: %s\nexp_res: %s\n", my_res, exp_res);
  //       }
  //     }
  //   }
  // }

  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  // set_scale(&max, 28);
  s21_decimal b = {{2, 0, 0, 0}};
  // set_scale(&b, 28);
  s21_decimal result;
  (*total_tests_count)++;
  int status = s21_mul(max, b, &result);

  if (status != INF)
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
    printf(" my_res: %d\nexp_res: %d\n", status, INF);
  }
  else
  {

    printf("TEST #%d PASSED!\n", *total_tests_count);
  }

  max = (s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  // set_scale(&max, 28);
  // set_sign(&max, 1);
  b = (s21_decimal){{2, 0, 0, 0}};
  set_sign(&b, 1);
  // set_scale(&b, 28);
  (*total_tests_count)++;
  status = s21_mul(max, b, &result);

  if (status != NEGATIVE_INF)
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
    printf(" my_res: %d\nexp_res: %d\n", status, NEGATIVE_INF);
  }
  else
  {

    printf("TEST #%d PASSED!\n", *total_tests_count);
  }

  return failed_tests_count;
}

int test_s21_div(int *total_tests_count)
{
  int failed_tests_count = 0;
  int values_a[10] = {12344, 5, -8, 6, 26, 10, -2};
  int values_b[10] = {12344, 5, -8, 29, 9, 10, -2};

  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 7; j++)
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

        sprintf(my_res, "%c%u / %c%u = %c%u\n", sign1, a.bit[0], sign2,
                b.bit[0], sign3, result.bit[0]);
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

int test_s21_is_greater(int *total_tests_count)
{
  s21_decimal decs1[6] = {{{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{1, 1, 0, 0}}, {{1, 0, 1, 0}}, {{1, 1, 1, 0}}};
  s21_decimal decs2[6] = {{{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{1, 1, 0, 0}}, {{1, 0, 1, 0}}, {{1, 1, 1, 0}}};
  const int scales[5] = {1, 3, 6, 8, 11};
  int failed_tests_count = 0;

  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 6; j++)
    {
      for (int s = 0; s < 5; s++)
      {
        for (int s2 = 0; s2 < 5; s2++)
        {
          (*total_tests_count)++;
          set_scale(&decs1[i], scales[s]);
          set_scale(&decs2[j], scales[s2]);
          char res[3] = "=<";
          if (s21_is_greater(decs1[i], decs2[j]))
            strcpy(res, ">");

          printf("Result: %d%d%d e%d %s %d%d%d e%d\n", decs1[i].bit[2], decs1[i].bit[1],
                 decs1[i].bit[0], get_scale(&decs1[i]), res, decs2[j].bit[2], decs2[j].bit[1],
                 decs2[j].bit[0], get_scale(&decs2[j]));
        }
      }
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

      printf("Result: %d%d%d %s %d%d%d\n", decs1[i].bit[2], decs1[i].bit[1],
             decs1[i].bit[0], res, decs2[j].bit[2], decs2[j].bit[1],
             decs2[j].bit[0]);
    }
  }

  s21_decimal a = {{0x1708, 0x0, 0x0, 0x80000000}}; //-5896
  s21_decimal b = {{0x1EC4, 0x0, 0x0, 0x80000000}}; //-7876

  if (!s21_is_greater_or_equal(a, b))
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);

    printf("Result: %d%d%d < %d%d%d\n", a.bit[2], a.bit[1],
           a.bit[0], b.bit[2], b.bit[1],
           b.bit[0]);
  }
  else
  {

    printf("TEST #%d passed!\n", *total_tests_count);
  }

  return failed_tests_count;
}

int test_s21_is_equal(int *total_tests_count)
{
  s21_decimal decs1[6] = {{{0, 0, 0, 0}}, {{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{1, 1, 0, 0}}, {{1, 0, 1, 0}}, {{1, 1, 1, 0}}};
  s21_decimal decs2[6] = {{{0, 0, 0, 0}}, {{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{1, 1, 0, 0}}, {{1, 0, 1, 0}}, {{1, 1, 1, 0}}};
  int failed_tests_count = 0;

  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 6; j++)
    {
      (*total_tests_count)++;

      char res[3] = "!=";
      if (s21_is_equal(decs1[i], decs2[j]))
        strcpy(res, "=");

      printf("Result: %d%d%d %s %d%d%d\n", decs1[i].bit[2], decs1[i].bit[1],
             decs1[i].bit[0], res, decs2[j].bit[2], decs2[j].bit[1],
             decs2[j].bit[0]);
    }
  }

  for (int i = 0; i < 6; i++)
  {
    set_sign(&decs1[i], 1);
    for (int j = 0; j < 6; j++)
    {
      (*total_tests_count)++;

      char res[3] = "!=";
      if (s21_is_equal(decs1[i], decs2[j]))
        strcpy(res, "=");

      printf("Result: %s%d%d%d %s %s%d%d%d\n", get_sign(&decs1[i]) ? "-" : "", decs1[i].bit[2], decs1[i].bit[1],
             decs1[i].bit[0], res, get_sign(&decs2[j]) ? "-" : "", decs2[j].bit[2], decs2[j].bit[1],
             decs2[j].bit[0]);
    }
  }
  s21_decimal a = {{0x809F4038, 0xAF56227, 0x0, 0x80050000}}; // -7896452314745.56984
  s21_decimal b = {{0x809F4038, 0xAF56227, 0x0, 0x80060000}}; // -789645231474.556984

  if (s21_is_equal(a, b))
  {
    failed_tests_count++;
    printf("TEST #%d FAILED!\n", *total_tests_count);
  }
  else
  {

    printf("TEST #%d passed!\n", *total_tests_count);
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
