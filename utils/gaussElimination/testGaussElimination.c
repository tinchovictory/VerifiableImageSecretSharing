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

  int ans = apply_gauss_elimination(mat);
  printf("gauss elimination ans: %d\n", ans);

  print_matrix(mat);

  free_matrix(mat);

  return 0;
}
