#ifndef __MATRIX_ARRAY_H__
#define __MATRIX_ARRAY_H__

#include "../matrix/matrix.h"

typedef struct matrix_array * matrix_array_t;

/*
 * Init matrix array of param size
 * If error return NULL
 */
matrix_array_t new_matrix_array(int size);

/*
 * Free matrix array, including all the matrix inside
 */
void free_matrix_array(matrix_array_t matrix_array);

/*
 * Add item to matrix array at position index
 * Return 1 on success, 0 on error
 */
int add_matrix_array(matrix_array_t matrix_array, const matrix_t item, int index);

/*
 * Get item of the array at position index
 * Return NULL on error
 */
matrix_t get_matrix_array_item(const matrix_array_t matrix_array, int index);

/*
 * Get matrix array size
 * Return -1 on error
 */
int get_matrix_array_size(const matrix_array_t matrix_array);

/*
 * Print matrix array
 */
void print_matrix_array(const matrix_array_t matrix_array, const char *str);

#endif
