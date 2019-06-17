#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"
#include "multiplicativeInverse.h"

#define MODULUS 251

struct matrix {
  int **matrix;
  int height; // m
  int width; // n
};


/* Helper functions declaration */
static matrix_t getCofactor(const matrix_t matrix, int row, int column, matrix_t cofactor);
static matrix_t getAdjoint(const matrix_t matrix);
static matrix_t applyModulus(matrix_t matrix);




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
 * Inverse param matrix
 * If matrix is not invertible return NULL
 */
matrix_t inverse_matrix(const matrix_t matrix) {
  matrix_t adjoint, inverse;

  int det = determinant_of_matrix(matrix);
  if(det == 0) {
    return NULL;
  }

  adjoint = getAdjoint(matrix);
  if(adjoint == NULL) {
    return NULL;
  }

  inverse = new_matrix(matrix->height, matrix->width);
  if(inverse == NULL) {
    free_matrix(adjoint);
    return NULL;
  }

  for(int i = 0; i < inverse->height; i++) {
    for(int j = 0; j < inverse->width; j++) {
      inverse->matrix[i][j] = adjoint->matrix[i][j] * multiplicative_inverse(det);
    }
  }

  free_matrix(adjoint);
  return applyModulus(inverse);
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

  return applyModulus(multiplied);
}


/*
 * Determinant of matrix
 * If out of memory error return -1
 * Otherwise return the determinant of the matrix
 */
int determinant_of_matrix(const matrix_t matrix) {
  matrix_t mat;
  int det = 1, total = 1, idx;

  /* Clone matrix to avoid changeing it */
  mat = clone_matrix(matrix);
  if(mat == NULL) {
    return -1;
  }

  /* Loop over the diagonal */
  for(int i = 0; i < mat->width; i++) {
    idx = i;

    /* Find the non-zero index */
    while(idx < mat->height && mat->matrix[idx][i] == 0) {
      idx++;
    }


    /* The determinant of the matrix is 0 */
    if(idx == mat->height) {
      free_matrix(mat);
      return 0;
    }

    /* Swap row */
    if(idx != i) {
      for(int j = 0; j < mat->width; j++) {
        int aux = mat->matrix[i][j];
        mat->matrix[i][j] = mat->matrix[idx][j];
        mat->matrix[idx][j] = aux;
      }
      /* Determinant sign changes when we shift rows */
      det *= (int) pow(-1, idx - i);
    }

    /* Loop over the rows below the diagonal */
    for(int j = i + 1; j < mat->height; j++) {
      int num = mat->matrix[j][i];

      /* Loop over the current row items */
      for(int k = 0; k < mat->width; k++) {
        /* Make the diagonal element and the next row equal */
        mat->matrix[j][k] = (mat->matrix[i][i] * mat->matrix[j][k]) - (num * mat->matrix[i][k]);
      }
      /* Det(kA) = k Det(A) */
      total *= mat->matrix[i][i];
    }

  }
  
  /* Multiply the diagonal elements */
  for(int i = 0; i < mat->width; i++) {
    det *= mat->matrix[i][i];
  }

  free_matrix(mat);

  return det/total;
}


/*
 * Substract matrixB from matrixA
 * matrixA - matrixB
 * Return new matrix with the operation
 */
matrix_t substract_matrix(const matrix_t matrixA, const matrix_t matrixB) {
  matrix_t substract;

  if(matrixA == NULL || matrixB == NULL) {
    return NULL;
  }

  if(matrixA->height != matrixB->height || matrixA->width != matrixB->width) {
    return NULL;
  }

  substract = clone_matrix(matrixA);
  if(substract == NULL) {
    return NULL;
  }

  for(int i = 0; i < substract->height; i++) {
    for(int j = 0; j < substract->width; j++) {
      substract->matrix[i][j] -= matrixB->matrix[i][j];
    }
  }

  return applyModulus(substract);
}


/*
 * Project matrix
 * Projection is (A x ( (A' x A) ^-1 ) x A')
 * Return NULL on error
 */
matrix_t project_matrix(const matrix_t matrix) {
  matrix_t projection = NULL, transpose = NULL, multipied = NULL, inverse = NULL;
  
  if(matrix == NULL) {
    return NULL;
  }

  /* Get A' */
  transpose = transpose_matrix(matrix);
  if(transpose == NULL) {
    goto out;
  }

  /* Get (A' x A) */
  multipied = multiply_matrix(transpose, matrix);
  if(multipied == NULL) {
    goto out;
  }

  /* Get (A' x A)^-1 */
  inverse = inverse_matrix(multipied);
  if(inverse == NULL) {
    goto out;
  }

  free_matrix(multipied);

  multipied = multiply_matrix(matrix, inverse);
  if(multipied == NULL) {
    goto out;
  }

  projection = multiply_matrix(multipied, transpose);

  /* Free up all matrix */
  out:
  free_matrix(transpose);
  free_matrix(multipied);
  free_matrix(inverse);

  return applyModulus(projection);
}



/*
 * Set value to matrix in position (i, j)
 * Returns 1 on success, 0 on error
 */
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

  while(value < 0) {
    value += MODULUS;
  }
  matrix->matrix[i][j] = value % MODULUS;

  return 1;
}


/*
 * Print matrix
 */
void print_matrix(matrix_t matrix) {
  if(matrix == NULL) {
    printf("Unable to print matrix, it's null\n");
    return;
  }

  for(int i = 0; i < matrix->height; i++) {
    for(int j = 0; j < matrix->width; j++) {
      printf("%4d ", matrix->matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}



/* Helper functions */

/* 
 * Get the cofactor matrix from the param matrix
 * Removing param row and param column
 * On error return NULL
 * Cofactor matrix is left in the cofactor param and also returned
 */
static matrix_t getCofactor(const matrix_t matrix, int row, int column, matrix_t cofactor) {
  if(matrix == NULL) {
    return NULL;
  }

  /* Loop over all rows and columns and copy them into param cofactor */
  /* Skip row and column from param */
  for(int i = 0, ic = 0; i < matrix->height; i++) {
    for(int j = 0, jc = 0; j < matrix->width; j++) {
      if(i != row && j != column) {
        cofactor->matrix[ic][jc++] = matrix->matrix[i][j];
      }
    }
    if(i != row) {
      ic++;
    }
  }

  return cofactor;
}

/*
 * Get the adjoint matrix from the param matrix
 * On error return NULL
 */
static matrix_t getAdjoint(const matrix_t matrix) {
  matrix_t adjoint, cofactor;
  int sign = 1;

  if(matrix == NULL) {
    return NULL;
  }

  /* Alloc adjoint matrix with the same size as param matrix */
  adjoint = new_matrix(matrix->height, matrix->width);
  if(adjoint == NULL) {
    return NULL;
  }

  /* Case matrix is of size 1 */
  if(matrix->width == 1) {
    adjoint->matrix[0][0] = 1;
    return adjoint;
  }

  /* Alloc cofactor matrix only once */
  cofactor = new_matrix(matrix->height - 1, matrix->width - 1);
  if(cofactor == NULL) {
    free_matrix(adjoint);
    return NULL;
  }

  for(int i = 0; i < matrix->height; i++) {
    for(int j = 0; j < matrix->width; j++) {
      getCofactor(matrix, i, j, cofactor);

      /* Sign is positive if sum of row and column idx is even */
      sign = (i + j) % 2 == 0 ? 1 : -1;

      /* Adjoint is the tranpose of the matrix */
      adjoint->matrix[j][i] = sign * determinant_of_matrix(cofactor);
    }
  }

  free_matrix(cofactor);
  return adjoint;
}

/*
 * Apply modulus to the param matrix in place
 */
static matrix_t applyModulus(matrix_t matrix) {
  if(matrix == NULL) {
    return NULL;
  }

  for(int i = 0; i < matrix->height; i++) {
    for(int j = 0; j < matrix->width; j++) {
      while(matrix->matrix[i][j] < 0) {
        matrix->matrix[i][j] += MODULUS;
      }
      matrix->matrix[i][j] %= MODULUS;
    }
  }

  return matrix;
}
