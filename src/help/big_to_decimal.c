#include "../s21_decimal.h"

int big_to_decimal(s21_big_decimal value, s21_decimal *result, int *scale) {
    int res = OK;
    int overflow = 0;
    int exp = *scale;
    s21_decimal c_res = {0};
    s21_big_decimal c_value = value;
    
    if ((value.b_bit[3] != 0 || value.b_bit[4] != 0 || value.b_bit[5] != 0 || value.b_bit[6] != 0 || value.b_bit[7 != 0]) && exp == 0) {
        res = INF;
    }

    if ((value.b_bit[3] != 0 && exp != 0) ||  exp > 28) {
        overflow = 1;
    }
    
    if (overflow == 0 && res == OK) {
        for (int i = 0; i <= 3; i++) {
            c_res.bit[i] = value.b_bit[i];
        }
    } else if (overflow == 1 && res = OK) {
        unsigned a = 0, iter = 0, flag = 0;
        while (exp != 0 && !(value.b_bit[3] == 0 && exp < 29)) {
            big_div10(&value, &a); // дописать
            if (a > 0) flag++;
            exp--;
            iter--;
        }

        if (flag) flag -= 1;

        bank_round(&value, c_value, iter, flag);
        if (value.b_bit[3] == 0) {
            for (int i = 0; i <= 2; i++) {
                c_res.bit[i] = value.b_bit[i];
            }
        }
    }

    if(is_zero(c_res)) res = INF;
    if(overflow == 1 && big_is_zero(value)) res = NEGATIVE_INF;

    set_scale(&c_res, exp);

    *result = c_res;
    *scale = exp;

    return res;
}
