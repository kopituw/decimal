#include "../s21_decimal.h"
#include <stdio.h>

int s21_from_decimal_to_int(s21_decimal src, int *dst) {

    int error_flag = 0;

    if (!dst) {
        error_flag = 1;
    } else {
        s21_decimal truncated;
        if (!error_flag) {
            int truncate_result = s21_truncate(src, &truncated);
            if (truncate_result != 0) {
                error_flag = 1;
            }
        }

        if (!error_flag) {
            if (get_sign(&truncated) == 0 && truncated.bit[0] > 2147483647) {
                error_flag = 1;
            } else if (get_sign(&truncated) == 1 && truncated.bit[0] > 2147483647u) {
                error_flag = 1;
            } else if (truncated.bit[1] != 0 || truncated.bit[2] != 0) {
                error_flag = 1;
            }
        }

        if (!error_flag) {
            *dst = (int)truncated.bit[0];
            if (truncated.bit[3] & (1 << 31)) {
                *dst = -(*dst);
            }
        }
    }

    return error_flag;
}