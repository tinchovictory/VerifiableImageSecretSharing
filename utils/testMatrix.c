#include <stdio.h>

#include "matrix.h"

int main(void) {
  matrix_t matrix = new_matrix(2, 2);

  set_matrix(matrix, 0, 0, 2);
  set_matrix(matrix, 0, 1, 4);
  set_matrix(matrix, 1, 0, 1);
  set_matrix(matrix, 1, 1, 4);

  print_matrix(matrix);

  matrix_t matB = clone_matrix(matrix);

  set_matrix(matB, 0, 0, 1);
  set_matrix(matB, 0, 1, 4);
  set_matrix(matB, 1, 0, 1);
  set_matrix(matB, 1, 1, 3);
  
  print_matrix(matB);

  matrix_t mult = multiply_matrix(matrix, matB);
  print_matrix(mult);

  matrix_t transpose = transpose_matrix(mult);
  print_matrix(transpose);

  free_matrix(matrix);
  free_matrix(matB);
  free_matrix(mult);
  free_matrix(transpose);




  // non square matrix

  matrix = new_matrix(2, 1);

  set_matrix(matrix, 0, 0, 2);
  set_matrix(matrix, 1, 0, 4);

  print_matrix(matrix);

  matB = new_matrix(1, 2);

  set_matrix(matB, 0, 0, 7);
  set_matrix(matB, 0, 1, 5);
  
  print_matrix(matB);

  mult = multiply_matrix(matrix, matB);
  print_matrix(mult);

  transpose = transpose_matrix(matB);
  print_matrix(transpose);

  free_matrix(matrix);
  free_matrix(matB);
  free_matrix(mult);
  free_matrix(transpose);

  return 0;
}