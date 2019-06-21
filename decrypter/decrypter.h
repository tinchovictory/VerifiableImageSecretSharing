#ifndef __DECRYPTER_H__
#define __DECRYPTER_H__

#include "../utils/matrix/matrix.h"
#include "../utils/matrixArray/matrixArray.h"

/*
 * Decrypter
 * Param shares is a matrix array of size k
 */
void decrypt(const matrix_array_t shares, const int sharesIdx[], const matrix_t remainder);

#endif
