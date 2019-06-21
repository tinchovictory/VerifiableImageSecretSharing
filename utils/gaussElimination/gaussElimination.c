#include <stdio.h>

#include "gaussElimination.h"
#include "../multiplicativeInverse/multiplicativeInverse.h"

static int swap_diagonal_row(matrix_t matrix, int diag);

/*
 * Apply gauss-jordan elimination to given matrix in place
 * It's assumed the matrix is M x N, with N = ( M + 1 )
 * The last column of the matrix represent the result values
 * Return 0 on error
 * Return 1 on success
 */
int apply_gauss_elimination(matrix_t matrix) {
  int diagInverse, value;

  /* Check for valid matrix */
  if(matrix == NULL) {
    return 0;
  }

  /* Iterate over  */
  for(int diag = 0; diag < get_matrix_width(matrix) - 1; diag++) {
    /* If diagonal value is 0 swap it with the next non-zero row */
    if(get_matrix(matrix, diag, diag) == 0) {
      if(!swap_diagonal_row(matrix, diag)) {
        return 0;
      }
    }

    /* Divide matrix by diagonal value */
    diagInverse = multiplicative_inverse(get_matrix(matrix, diag, diag));
    for(int j = 0; j < get_matrix_width(matrix); j++) {
      value = get_matrix(matrix, diag, j) * diagInverse;
      set_matrix(matrix, diag, j, value);
    }

    /* Substract current row to all other rows */
    for(int i = 0; i < get_matrix_height(matrix); i++) {
      /* Skip diagonal row */
      if(i != diag) {
        /* Iterate over all row and substract diagonal value */
        for(int j = 0; j < get_matrix_width(matrix); j++) {
          value = get_matrix(matrix, i, j) - get_matrix(matrix, diag, j) * get_matrix(matrix, i, diag);
          set_matrix(matrix, i, j, value);
        }
      }
    }
  }

  return 1;
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
