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


  // test determinant

  matrix = new_matrix(4, 4);
  set_matrix(matrix, 0, 0, 1);
  set_matrix(matrix, 0, 1, 0);
  set_matrix(matrix, 0, 2, 2);
  set_matrix(matrix, 0, 3, -1);
  set_matrix(matrix, 1, 0, 3);
  set_matrix(matrix, 1, 1, 0);
  set_matrix(matrix, 1, 2, 0);
  set_matrix(matrix, 1, 3, 5);
  set_matrix(matrix, 2, 0, 2);
  set_matrix(matrix, 2, 1, 1);
  set_matrix(matrix, 2, 2, 4);
  set_matrix(matrix, 2, 3, -3);
  set_matrix(matrix, 3, 0, 1);
  set_matrix(matrix, 3, 1, 0);
  set_matrix(matrix, 3, 2, 5);
  set_matrix(matrix, 3, 3, 0);

  print_matrix(matrix);

  printf("Determinat of matrix is %d\n", determinant_of_matrix(matrix));

  // free_matrix(matrix);

  // test inverse
  // matrix = new_matrix(4, 4);
  // set_matrix(matrix, 0, 0, 5);
  // set_matrix(matrix, 0, 1, -2);
  // set_matrix(matrix, 0, 2, 2);
  // set_matrix(matrix, 0, 3, 7);
  // set_matrix(matrix, 1, 0, 1);
  // set_matrix(matrix, 1, 1, 0);
  // set_matrix(matrix, 1, 2, 0);
  // set_matrix(matrix, 1, 3, 3);
  // set_matrix(matrix, 2, 0, -3);
  // set_matrix(matrix, 2, 1, 1);
  // set_matrix(matrix, 2, 2, 5);
  // set_matrix(matrix, 2, 3, 0);
  // set_matrix(matrix, 3, 0, 3);
  // set_matrix(matrix, 3, 1, -1);
  // set_matrix(matrix, 3, 2, -9);
  // set_matrix(matrix, 3, 3, 4);

  print_matrix(matrix);
  matrix_t inverse = inverse_matrix(matrix);

  print_matrix(inverse);

  free_matrix(inverse);
  free_matrix(matrix);


  // Test substract
  matrix = new_matrix(2, 2);
  matB = new_matrix(2, 2);
  set_matrix(matrix, 0, 0, 1);
  set_matrix(matrix, 0, 1, 5);
  set_matrix(matrix, 1, 0, 3);
  set_matrix(matrix, 1, 1, 2);
  print_matrix(matrix);
  
  set_matrix(matB, 0, 0, 0);
  set_matrix(matB, 0, 1, 8);
  set_matrix(matB, 1, 0, 2);
  set_matrix(matB, 1, 1, 1);
  print_matrix(matB);

  matrix_t substract = substract_matrix(matrix, matB);
  print_matrix(substract);

  free_matrix(matrix);
  free_matrix(matB);
  free_matrix(substract);


  // Test project
  matrix = new_matrix(4, 2);
  set_matrix(matrix, 0, 0, 3);
  set_matrix(matrix, 0, 1, 7);
  set_matrix(matrix, 1, 0, 6);
  set_matrix(matrix, 1, 1, 1);
  set_matrix(matrix, 2, 0, 2);
  set_matrix(matrix, 2, 1, 5);
  set_matrix(matrix, 3, 0, 6);
  set_matrix(matrix, 3, 1, 6);

  print_matrix(matrix);
  matrix_t projection = project_matrix(matrix);
  print_matrix(projection);

  free_matrix(matrix);
  free_matrix(projection);



  // Test inverse
  matrix = new_matrix(4, 4);
  set_matrix(matrix, 0, 0, 230);
  set_matrix(matrix, 0, 1, 5);
  set_matrix(matrix, 0, 2, 42);
  set_matrix(matrix, 0, 3, 75);
  set_matrix(matrix, 1, 0, 5);
  set_matrix(matrix, 1, 1, 200);
  set_matrix(matrix, 1, 2, 78);
  set_matrix(matrix, 1, 3, 51);
  set_matrix(matrix, 2, 0, 42);
  set_matrix(matrix, 2, 1, 78);
  set_matrix(matrix, 2, 2, 84);
  set_matrix(matrix, 2, 3, 183);
  set_matrix(matrix, 3, 0, 75);
  set_matrix(matrix, 3, 1, 51);
  set_matrix(matrix, 3, 2, 183);
  set_matrix(matrix, 3, 3, 182);

  print_matrix(matrix);
  inverse = inverse_matrix(matrix);
  print_matrix(inverse);

  free_matrix(matrix);
  free_matrix(inverse);

  return 0;
}
