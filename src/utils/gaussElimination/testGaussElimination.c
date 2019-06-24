#include <stdio.h>

#include "gaussElimination.h"
#include "../matrix/matrix.h"

int main(void) {

  /* Build matrix */
  matrix_t mat = new_matrix(3, 4);
  set_matrix(mat, 0, 0, 0);
  set_matrix(mat, 0, 1, 2);
  set_matrix(mat, 0, 2, 1);
  set_matrix(mat, 0, 3, 4);
  set_matrix(mat, 1, 0, 1);
  set_matrix(mat, 1, 1, 1);
  set_matrix(mat, 1, 2, 2);
  set_matrix(mat, 1, 3, 6);
  set_matrix(mat, 2, 0, 2);
  set_matrix(mat, 2, 1, 1);
  set_matrix(mat, 2, 2, 1);
  set_matrix(mat, 2, 3, 7);

  /* Print matrix */
  print_matrix(mat);

  matrix_t ans = apply_gauss_elimination(mat, get_matrix_width(mat) - 1);

  print_matrix(ans);

  free_matrix(mat);
  free_matrix(ans);


  // Test paper
  mat = new_matrix(2, 3);
  set_matrix(mat, 0, 0, 1);
  set_matrix(mat, 0, 1, 1);
  set_matrix(mat, 0, 2, 36);
  set_matrix(mat, 1, 0, 1);
  set_matrix(mat, 1, 1, 2);
  set_matrix(mat, 1, 2, 71);
  
  print_matrix(mat);

  ans = apply_gauss_elimination(mat, get_matrix_width(mat) - 1);

  print_matrix(ans);

  free_matrix(mat);
  free_matrix(ans);


  return 0;
}
