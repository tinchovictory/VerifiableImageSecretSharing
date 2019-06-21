#ifndef __DECRYPTER_H__
#define __DECRYPTER_H__

#include "../utils/matrix/matrix.h"
#include "../utils/matrixArray/matrixArray.h"

struct decrypt_output {
  matrix_t secret;
  matrix_t watermark;
};

/*
 * Decrypter
 * Param shares is a matrix array of size k
 */
struct decrypt_output decrypt(const matrix_array_t shares, const int sharesIdx[], const matrix_t remainder);

#endif
