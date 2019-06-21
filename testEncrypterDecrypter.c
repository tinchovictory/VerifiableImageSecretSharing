#include <stdio.h>
#include <stdlib.h>

#include "utils/matrix/matrix.h"
#include "utils/matrixArray/matrixArray.h"
#include "encrypter/encrypter.h"
#include "decrypter/decrypter.h"

static matrix_t readImage();
static matrix_t readWatermark();

int main(void) {
  matrix_t image, watermark;

  image = readImage();
  watermark = readWatermark();
  if(image == NULL || watermark == NULL) {
    return 1;
  }

  printf("--- Starting encryption ---\n");

  struct encrypt_output encryptOutput = encrypt(image, watermark, 2, 4);

  if(encryptOutput.shares == NULL || encryptOutput.sharesIdx == NULL || encryptOutput.remainder == NULL) {
    printf("Something failed in the encryption\n");
    return 1;
  }

  printf("Mat Rw is:\n");
  print_matrix(encryptOutput.remainder);
  print_matrix_array(encryptOutput.shares, "Sh");


  printf("--- Starting decryption ---\n");

  /* Remove elements from the shares array */
  matrix_array_t checkShares = new_matrix_array(2);
  add_matrix_array(checkShares, get_matrix_array_item(encryptOutput.shares, 0), 0);
  add_matrix_array(checkShares, get_matrix_array_item(encryptOutput.shares, 1), 1);
  int sharesIdx[4] = {1, 2};


  struct decrypt_output decryptOutput = decrypt(checkShares, sharesIdx, encryptOutput.remainder);
  
  if(decryptOutput.secret == NULL || decryptOutput.watermark == NULL) {
    printf("Something failed in the decryption\n");
    return 1;
  }

  printf("Mat S is \n");
  print_matrix(decryptOutput.secret);
  printf("Mat W is \n");
  print_matrix(decryptOutput.watermark);


  /* Output */
  printf("--- Output ---\n");
  printf("The image is: %s\n", are_matrices_equal(image, decryptOutput.secret) == 1 ? "equal" : "not equal");
  printf("The Watermark is: %s\n", are_matrices_equal(watermark, decryptOutput.watermark) == 1 ? "equal" : "not equal");

  free_matrix_array(encryptOutput.shares);
  free(encryptOutput.sharesIdx);
  free_matrix(encryptOutput.remainder);
  free_matrix(decryptOutput.secret);
  free_matrix(decryptOutput.watermark);

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
