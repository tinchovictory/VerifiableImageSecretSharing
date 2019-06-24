#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"
#include "../multiplicativeInverse/multiplicativeInverse.h"
#include "../gaussElimination/gaussElimination.h"

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
  if(matrix == NULL) {
    return -1;
  }

  int det = 0;

  /* Base case */
  if(matrix->width == 1) {
    return matrix->matrix[0][0];
  }

  int sign = 1;

  /* Alloc cofactor matrix only once */
  matrix_t cofactor = new_matrix(matrix->height - 1, matrix->width - 1);
  if(cofactor == NULL) {
    return 0;
  }

  for(int j = 0; j < matrix->width; j++) {
    getCofactor(matrix, 0, j, cofactor);
    det += sign * matrix->matrix[0][j] * determinant_of_matrix(cofactor);
    det %= MODULUS;
    sign = -sign;
  }

  return det;
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
 * Add matrixA to matrixB
 * matrixA + matrixB
 * Return new matrix with the operation
 * Return NULL on error
 */
matrix_t add_matrix(const matrix_t matrixA, const matrix_t matrixB) {
  matrix_t add;

  if(matrixA == NULL || matrixB == NULL) {
    return NULL;
  }

  if(matrixA->height != matrixB->height || matrixA->width != matrixB->width) {
    return NULL;
  }

  add = clone_matrix(matrixA);
  if(add == NULL) {
    return NULL;
  }

  for(int i = 0; i < add->height; i++) {
    for(int j = 0; j < add->width; j++) {
      add->matrix[i][j] += matrixB->matrix[i][j];
    }
  }

  return applyModulus(add);
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

  // printf("transposed\n");

  /* Get (A' x A) */
  multipied = multiply_matrix(transpose, matrix);
  if(multipied == NULL) {
    goto out;
  }
  // printf("multiplied\n");

  /* Get (A' x A)^-1 */
  inverse = inverse_matrix(multipied);
  if(inverse == NULL) {
    goto out;
  }
  // printf("inversed\n");

  free_matrix(multipied);

  multipied = multiply_matrix(matrix, inverse);
  if(multipied == NULL) {
    goto out;
  }
  // printf("multiplied\n");

  projection = multiply_matrix(multipied, transpose);

  // printf("projected\n");
  /* Free up all matrix */
  out:
  free_matrix(transpose);
  free_matrix(multipied);
  free_matrix(inverse);

  return applyModulus(projection);
}


/*
 * Concatenate matrixA | matrixB
 * Return new matrix with new size and values
 * Return NULL on error
 */
matrix_t concatenate_matrix(const matrix_t matrixA, const matrix_t matrixB) {
  matrix_t concatenate;

  if(matrixA == NULL || matrixB == NULL) {
    return NULL;
  }

  if(matrixA->height != matrixB->height) {
    return NULL;
  }

  /* Allocate memory */
  concatenate = new_matrix(matrixA->height, matrixA->width + matrixB->width);
  if(concatenate == NULL) {
    return NULL;
  }

  /* Copy matrix A into concatenate */
  for(int i = 0; i < concatenate->height; i++) {
    for(int j = 0; j < matrixA->width; j++) {
      concatenate->matrix[i][j] = matrixA->matrix[i][j];
    }
  }
  
  /* Copy matrix B into concatenate */
  for(int i = 0; i < concatenate->height; i++) {
    for(int j = 0; j < matrixB->width; j++) {
      concatenate->matrix[i][j + matrixA->width] = matrixB->matrix[i][j];
    }
  }

  return concatenate;
}


/*
 * Getter of the value in the position (i, j)
 * If position is not valid return -1
 */
int get_matrix(const matrix_t matrix, int i, int j) {
  if(matrix == NULL) {
    return -1;
  }
  if(i < 0 || i > matrix->height) {
    return -1;
  }
  if(j < 0 || j > matrix->width) {
    return -1;
  }

  return matrix->matrix[i][j];
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
 * Matrix height getter
 * On error return -1
 */
int get_matrix_height(const matrix_t matrix) {
  if(matrix == NULL) {
    return -1;
  }

  return matrix->height;
}

/*
 * Matrix width getter
 * On error return -1
 */
int get_matrix_width(const matrix_t matrix) {
  if(matrix == NULL) {
    return -1;
  }

  return matrix->width;
}


/*
 * Print matrix
 */
void print_matrix(const matrix_t matrix) {
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


/*
 * Check if both matrix are equal
 * Return 0 on false
 */
int are_matrices_equal(const matrix_t matrixA, const matrix_t matrixB) {
  if(matrixA == NULL || matrixB == NULL) {
    return 0;
  }

  if(matrixA->height != matrixB->height || matrixA->width != matrixB->width) {
    return 0;
  }

  for(int i = 0; i < matrixA->height; i++) {
    for(int j = 0; j < matrixA->width; j++) {
      if(matrixA->matrix[i][j] != matrixB->matrix[i][j]) {
        return 0;
      }
    }
  }
  
  return 1;
}


/*
 * Check if matrix is full rank
 * Return 0 on false
 */
int is_matrix_full_rank(const matrix_t matrix) {
  matrix_t gauss;
  int count = 0;

  if(matrix == NULL) {
    return 0;
  }

  /* Apply gauss elimination to matrix */
  gauss = apply_gauss_elimination(matrix, matrix->width);
  if(gauss == NULL) {
    return 0;
  }

  /* Count the non-empty rows of the matrix */
  for(int i = 0; i < gauss->height; i++) {
    int sum = 0;
    for(int j = 0; j < gauss->width && !sum; j++) {
      sum += gauss->matrix[i][j];
    }
    if(sum != 0) {
      count++;
    }
  }

  free_matrix(gauss);

  if(matrix->width < matrix->height) {
    return count == matrix->width;
  }
  return count == matrix->height;
}

/*
 * Check if matrix is invertible
 * Return 0 on false
 */
int is_matrix_invertible(const matrix_t matrix) {
  return determinant_of_matrix(matrix) != 0;
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
