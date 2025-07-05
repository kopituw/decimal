#include <stdio.h>
#include <stdint.h>
#include "s21_decimal.h"

void print_bits(uint32_t value) {
    for (int i = 31; i >= 0; --i) {
      printf("%d", (value >> i) & 1);
    }
  }
  
void print_decimal(s21_decimal dec) {
   // printf("Result num: %d\n", dec.bits[0]);
    
    printf("Bits:\n");
    for (int i = 0; i < 4; ++i) {
      printf("bits[%d]: ", i);
      print_bits(dec.bits[i]);
      printf("\n");
    }
  
    // Извлекаем scale и знак из bits[3]
    uint32_t bits3 = dec.bits[3];
  
    int scale = (bits3 >> 16) & 0xFF;
    int sign = (bits3 >> 31) & 1;
  
    printf("Scale (decimal places): %d\n", scale);
    printf("Sign: %s\n", sign ? "negative" : "positive");
}
