#include <stdio.h>

#include "stenography.h"

int main(void) {
  unsigned char initialArray[] = {0xED, 0xA4, 0x45, 0x36, 0x3A, 0x3A, 0x3B, 0x39};
  unsigned char hiddenArray[] = {0xED, 0xA4, 0x45, 0x36, 0x3A, 0x3A, 0x3B, 0x39};

  unsigned char byte = 0xD1;

  int ans = stenography_hide(hiddenArray, 8, byte, 1);
  printf("ans is %d\n", ans);

  for(int i = 0; i < 8; i++) {
    printf("Original value: %x, Hidden value: %x\n", initialArray[i], hiddenArray[i]);
  }

  unsigned char hiddenByte = stenography_get(hiddenArray, 8, 1);

  printf("Hidden value is %x\n", hiddenByte);



  // Test hidding two bits

  unsigned char initialArray1[] = {0xED, 0xA4, 0x45, 0x36};
  unsigned char hiddenArray1[] = {0xED, 0xA4, 0x45, 0x36};

  byte = 0xD1;

  ans = stenography_hide(hiddenArray1, 4, byte, 2);
  printf("ans is %d\n", ans);

  for(int i = 0; i < 4; i++) {
    printf("Original value: %x, Hidden value: %x\n", initialArray1[i], hiddenArray1[i]);
  }

  hiddenByte = stenography_get(hiddenArray1, 4, 2);

  printf("Hidden value is %x\n", hiddenByte);


  return 0;
}
