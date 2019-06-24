#ifndef __ENCRYPTER_H__
#define __ENCRYPTER_H__

#include "../utils/matrix/matrix.h"
#include "../utils/matrixArray/matrixArray.h"

struct encrypt_output {
  matrix_array_t shares;
  int *sharesIdx;
  matrix_t remainder;
};

/*
 * Encrypter
 */
struct encrypt_output encrypt(const matrix_t image, const matrix_t watermark, int k, int n);

/*
 * Free encrypt_output
 */
void free_encrypt_output(struct encrypt_output output);

#endif
