#include <stdio.h>
#include <stdlib.h>

#include "matrixArray.h"

struct matrix_array {
  matrix_t *array;
  int size;
};


/*
 * Init matrix array of param size
 * If error return NULL
 */
matrix_array_t new_matrix_array(int size) {
  matrix_array_t matrix_array = NULL;

  /* Allocate matrix array structure */
  matrix_array = (matrix_array_t) malloc(sizeof(struct matrix_array));
  if(matrix_array == NULL) {
    return NULL;
  }
  
  /* Allocate internal structure array */
  matrix_array->array = (matrix_t *) calloc(size, sizeof(matrix_t));
  if(matrix_array->array == NULL) {
    free(matrix_array);
    return NULL;
  }

  /* Set size to structure */
  matrix_array->size = size;

  return matrix_array;
}


/*
 * Free matrix array, including all the matrix inside
 */
void free_matrix_array(matrix_array_t matrix_array) {
  if(matrix_array == NULL) {
    return;
  }

  for(int i = 0; i < matrix_array->size; i++) {
    free_matrix(matrix_array->array[i]);
  }
  free(matrix_array->array);
  free(matrix_array);
}

/*
 * Add item to matrix array at position index
 * Return 1 on success, 0 on error
 */
int add_matrix_array(matrix_array_t matrix_array, const matrix_t item, int index) {
  if(matrix_array == NULL || item == NULL) {
    return 0;
  }

  if(index < 0 || index >= matrix_array->size) {
    return 0;
  }

  matrix_array->array[index] = item;
  
  return 1;
}

/*
 * Get item of the array at position index
 * Return NULL on error
 */
matrix_t get_matrix_array_item(const matrix_array_t matrix_array, int index) {
  if(matrix_array == NULL) {
    return NULL;
  }

  if(index < 0 || index >= matrix_array->size) {
    return NULL;
  }

  return matrix_array->array[index];
}

/*
 * Get matrix array size
 * Return -1 on error
 */
int get_matrix_array_size(const matrix_array_t matrix_array) {
  if(matrix_array == NULL) {
    return -1;
  }

  return matrix_array->size;
}

/*
 * Print matrix array
 */
void print_matrix_array(const matrix_array_t matrix_array, const char *str) {
  if(matrix_array == NULL) {
    return;
  }

  printf("Printing matrix array %s\n", str);

  for(int i = 0; i < matrix_array->size; i++) {
    printf("Mat %s%d is:\n", str, i);

    print_matrix(matrix_array->array[i]);
  }
}
