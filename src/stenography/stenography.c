#include <stdio.h>
#include <math.h>

#include "stenography.h"

#define BITS_PER_BYTE 8

/*
 * Hide value into the array
 * The size of the array is given in the array_size param
 * The byte param is the value to hide in the array
 * The bits_per_item are the amount of bits to hide in each array item
 * 
 * The array_size should be equal to 8 / bits_per_item
 * 
 * On error return 0
 */
int stenography_hide(unsigned char *array, int array_size, unsigned char byte, int bits_per_item) {
  unsigned char bits_to_hide, bit_mask;

  if(array == NULL) {
    return 0;
  }

  if(array_size != BITS_PER_BYTE / bits_per_item) {
    return 0;
  }

  /* Build the bit mask depending on the bits_per_item */
  bit_mask = (unsigned char) (pow(2, bits_per_item) - 1);

  /* Iterate over the array hidding the value */
  for(int i = 0; i < array_size; i++) {
    /* Get the bits to hide in the current array item */
    bits_to_hide = byte >> (BITS_PER_BYTE - (i + 1) * bits_per_item);
    bits_to_hide &= bit_mask;

    /* Set the bits to the array item */
    array[i] |= bit_mask;
    array[i] &= (~bit_mask | bits_to_hide);
  }

  return 1;
}


/*
 * Get the value hidden in the array
 * The size of the array is given in the array_size param
 * The bits_per_item are the amount of bits hidden in each array item
 * 
 * The array_size should be equal to 8 / bits_per_item
 * 
 */
unsigned char stenography_get(unsigned char *array, int array_size, int bits_per_item) {
  unsigned char byte = 0, bit_mask, hidden_bits;

  if(array == NULL) {
    return 0;
  }

  if(array_size != BITS_PER_BYTE / bits_per_item) {
    return 0;
  }

  /* Build the bit mask depending on the bits_per_item */
  bit_mask = (unsigned char) (pow(2, bits_per_item) - 1);

  /* Iterate over the array getting the value */
  for(int i = 0; i < array_size; i++) {
    /* Get the bits from the current item */
    hidden_bits = array[i] & bit_mask;

    /* Add the bits to the byte */
    byte = (byte << bits_per_item) | hidden_bits;
  }

  return byte;
}
