#include <stdio.h>

#include "random.h"

/* Global seed value */
int64_t seed;

void set_seed(int64_t s) {
  seed = (s ^ 0x5DEECE66DL) & ((1LL << 48) - 1);
}

uint8_t next_char() {
  seed = (seed * 0x5DEECE66DL + 0xBL) & ((1LL << 48) - 1);
  return (uint8_t)(seed>>40);
}
