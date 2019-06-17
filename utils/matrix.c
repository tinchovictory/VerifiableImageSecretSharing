#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

struct matrix {
  int **matrix;
  int height; // m
  int width; // n
};


/*
 * Init a new matrix of size width x length
 * If dimesions are invalid returns NULL
 * If unable to allocate memory returns NULL
 */
matrix_t new_matrix(int height, int width) {
  matrix_t matrix;

  /* Validate parameters */
  if(width <= 0 || height <= 0) {
    return NULL;
  }

  /* Alloc matrix structure */
  matrix = (matrix_t) malloc(sizeof(struct matrix));
  if(matrix == NULL) {
    return NULL;
  }

  /* Init matrix structure */
  matrix->width = width;
  matrix->height = height;

  /* Init inner matrix data structure */
  matrix->matrix = (int **) malloc(sizeof(int *) * height);
  if(matrix->matrix == NULL) {
    free(matrix);
    return NULL;
  }

  for(int i = 0; i < height; i++) {
    matrix->matrix[i] = calloc(width, sizeof(int));
    if(matrix->matrix[i] == NULL) {
      free_matrix(matrix);
      return NULL;
    }
  }

  return matrix;
}

/*
 * Free up a matrix
 */
void free_matrix(matrix_t matrix) {
  if(matrix == NULL) {
    return;
  }

  /* Free inner matrix data structure */
  if(matrix->matrix != NULL) {
    for(int i = 0; i < matrix->height; i++) {
        free(matrix->matrix[i]);
    }

    free(matrix->matrix);
  }

  free(matrix);
}

/*
 * Clone param matrix
 */
matrix_t clone_matrix(const matrix_t matrix) {
  matrix_t clone;

  if(matrix == NULL) {
    return NULL;
  }

  /* Allocate new matrix */
  clone = new_matrix(matrix->height, matrix->width);
  if(clone == NULL) {
    return NULL;
  }

  /* Copy inner matrix values */
  for(int i = 0; i < clone->height; i++) {
    for(int j = 0; j < clone->width; j++) {
      clone->matrix[i][j] = matrix->matrix[i][j];
    }
  }

  return clone;
}

/*
 * Transpose param matrix
 * Return new matrix transposed. Original matrix is not free up
 */
matrix_t transpose_matrix(const matrix_t matrix) {
  matrix_t transpose;

  if(matrix == NULL) {
    return NULL;
  }

  /* Allocate new matrix */
  transpose = new_matrix(matrix->width, matrix->height);
  if(transpose == NULL) {
    return NULL;
  }

  /* Copy inner matrix values tansposed */
  for(int i = 0; i < transpose->height; i++) {
    for(int j = 0; j < transpose->width; j++) {
      transpose->matrix[i][j] = matrix->matrix[j][i];
    }
  }

  return transpose;
}

/*
 * Inverse param matrix in place
 * If matrix is not invertible return 0
 * On success return 1
 */
int inverse_matrix(matrix_t matrix) {
  return 0;
}

/*
 * Multiply param matrixA with param matrixB
 * Return multiplied matrix
 */
matrix_t multiply_matrix(const matrix_t matrixA, const matrix_t matrixB) {
  matrix_t multiplied;

  if(matrixA == NULL || matrixB == NULL) {
    return NULL;
  }

  /* Validate A x B have valid sizes */
  if(matrixA->width != matrixB->height) {
    return NULL;
  }

  /* Allocate new matrix memory */
  multiplied = new_matrix(matrixA->height, matrixB->width);
  if(multiplied == NULL) {
    return NULL;
  }

  /* Multiplication algorithm */
  for(int i = 0; i < matrixA->height; i++) {
    for(int j = 0; j < matrixB->width; j++) {
      for(int k = 0; k < matrixA->width; k++) {
        multiplied->matrix[i][j] += (matrixA->matrix[i][k] * matrixB->matrix[k][j]);
      }
    }
  }

  return multiplied;
}


int set_matrix(matrix_t matrix, int i, int j, int value) {
  if(matrix == NULL) {
    return 0;
  }
  
  if(i < 0 || i >= matrix->height) {
    return 0;
  }

  if(j < 0 || j >= matrix->width) {
    return 0;
  }

  matrix->matrix[i][j] = value;

  return 1;
}

void print_matrix(matrix_t matrix) {
  if(matrix == NULL) {
    printf("Unable to print matrix, it's null\n");
    return;
  }

  for(int i = 0; i < matrix->height; i++) {
    for(int j = 0; j < matrix->width; j++) {
      printf("%d ", matrix->matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
