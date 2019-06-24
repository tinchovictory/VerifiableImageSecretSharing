#include <stdio.h>

#include "gaussElimination.h"
#include "../multiplicativeInverse/multiplicativeInverse.h"

static int swap_diagonal_row(matrix_t matrix, int diag);

/*
 * Apply gauss-jordan elimination to given matrix
 * It's assumed the matrix is M x N
 * The width param defines the amount of columns to use in the elimination
 * Return NULL on error
 */
matrix_t apply_gauss_elimination(const matrix_t matrix, int width) {
  int diagInverse, value;
  matrix_t gauss = clone_matrix(matrix);

  /* Check for valid matrix */
  if(gauss == NULL) {
    return 0;
  }

  /* Iterate over */
  for(int diag = 0; diag < width; diag++) {
    /* If diagonal value is 0 swap it with the next non-zero row */
    if(get_matrix(gauss, diag, diag) == 0) {
      if(!swap_diagonal_row(gauss, diag)) {
        free_matrix(gauss);
        return NULL;
      }
    }

    /* Divide matrix by diagonal value */
    diagInverse = multiplicative_inverse(get_matrix(gauss, diag, diag));
    for(int j = 0; j < get_matrix_width(gauss); j++) {
      value = get_matrix(gauss, diag, j) * diagInverse;
      set_matrix(gauss, diag, j, value);
    }


    /* Substract current row to all other rows */
    for(int i = 0; i < get_matrix_height(gauss); i++) {
      /* Skip diagonal row */
      if(i != diag) {
        /* Iterate over all row and substract diagonal value */
        int factor = get_matrix(gauss, i, diag);
        for(int j = 0; j < get_matrix_width(gauss); j++) {
          value = get_matrix(gauss, i, j) - get_matrix(gauss, diag, j) * factor;
          set_matrix(gauss, i, j, value);
        }
      }
    }
  }

  return gauss;
}


/* Helper functions */

/* Find the next non-zero row, in the given column
 * If found swap it in the matrix param
 * On success return1
 * On error return 0
 */
static int swap_diagonal_row(matrix_t matrix, int diag) {
  int i, aux;
      
  /* Start from the next row */
  i = diag + 1;

  /* Search the next non-zero row */
  while(i < get_matrix_height(matrix) && get_matrix(matrix, i, diag) == 0) {
    i++;
  }

  /* Check if it's a valid row */
  if(i == get_matrix_height(matrix)) {
    return 0;
  }

  /* Swap  */
  for(int j = 0; j < get_matrix_width(matrix); j++) {
    aux = get_matrix(matrix, i, j);
    set_matrix(matrix, i, j, get_matrix(matrix, diag, j));
    set_matrix(matrix, diag, j, aux);
  }

  return 1;
}
