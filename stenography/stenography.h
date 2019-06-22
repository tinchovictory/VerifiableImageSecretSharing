#ifndef __STENOGRAPHY_H__
#define __STENOGRAPHY_H__

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
int stenography_hide(unsigned char *array, int array_size, unsigned char byte, int bits_per_item);


/*
 * Get the value hidden in the array
 * The size of the array is given in the array_size param
 * The bits_per_item are the amount of bits hidden in each array item
 * 
 * The array_size should be equal to 8 / bits_per_item
 * 
 */
unsigned char stenography_get(unsigned char *array, int array_size, int bits_per_item);

#endif
