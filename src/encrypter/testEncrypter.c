#include <stdio.h>
#include <stdlib.h>

#include "../utils/matrix/matrix.h"
#include "../utils/matrixArray/matrixArray.h"
#include "../utils/random/random.h"
#include "encrypter.h"

static matrix_t readImage();
static matrix_t readWatermark();

int main(void) {
  matrix_t image, watermark;

  set_seed(10);

  image = readImage();
  watermark = readWatermark();
  if(image == NULL || watermark == NULL) {
    return 1;
  }

  struct encrypt_output output = encrypt(image, watermark, 2, 4);

  if(output.shares == NULL || output.remainder == NULL) {
    printf("Something failed\n");
    return 1;
  }

  printf("Mat Rw is:\n");
  print_matrix(output.remainder);
  print_matrix_array(output.shares, "Sh");
  printf("Shares Idx: [");
  for(int i = 0; i < 4; i++) {
    printf("%d ", output.sharesIdx[i]);
  }
  printf("]\n");

  free_matrix_array(output.shares);
  free(output.sharesIdx);
  free_matrix(output.remainder);

  return 0;
}


static matrix_t readImage() {
  matrix_t matrix = new_matrix(4, 4);
  if(matrix == NULL) {
    return NULL;
  }

  set_matrix(matrix, 0, 0, 2);
  set_matrix(matrix, 0, 1, 5);
  set_matrix(matrix, 0, 2, 2);
  set_matrix(matrix, 0, 3, 3);
  set_matrix(matrix, 1, 0, 3);
  set_matrix(matrix, 1, 1, 6);
  set_matrix(matrix, 1, 2, 4);
  set_matrix(matrix, 1, 3, 5);
  set_matrix(matrix, 2, 0, 4);
  set_matrix(matrix, 2, 1, 7);
  set_matrix(matrix, 2, 2, 4);
  set_matrix(matrix, 2, 3, 6);
  set_matrix(matrix, 3, 0, 1);
  set_matrix(matrix, 3, 1, 4);
  set_matrix(matrix, 3, 2, 1);
  set_matrix(matrix, 3, 3, 7);

  return matrix;
}

static matrix_t readWatermark() {
  matrix_t matrix = new_matrix(4, 4);
  if(matrix == NULL) {
    return NULL;
  }

  set_matrix(matrix, 0, 0, 50);
  set_matrix(matrix, 0, 1, 100);
  set_matrix(matrix, 0, 2, 21);
  set_matrix(matrix, 0, 3, 14);
  set_matrix(matrix, 1, 0, 22);
  set_matrix(matrix, 1, 1, 76);
  set_matrix(matrix, 1, 2, 200);
  set_matrix(matrix, 1, 3, 54);
  set_matrix(matrix, 2, 0, 1);
  set_matrix(matrix, 2, 1, 91);
  set_matrix(matrix, 2, 2, 45);
  set_matrix(matrix, 2, 3, 7);
  set_matrix(matrix, 3, 0, 24);
  set_matrix(matrix, 3, 1, 66);
  set_matrix(matrix, 3, 2, 96);
  set_matrix(matrix, 3, 3, 120);

  return matrix;
}
