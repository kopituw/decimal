#include "../s21_decimal.h"

int get_sign(const s21_decimal *value) {
    uint32_t servicebit = value->bit[3]; // берем значение служебного бита 
    int sign = (servicebit >> 31) & 1; // сдвигаем вправо, чтобы получить интовое значение знака, если оператор = 1 это -, 0 это + 
    return sign;
}
