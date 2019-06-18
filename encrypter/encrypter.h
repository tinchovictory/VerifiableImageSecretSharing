#ifndef __ENCRYPTER_H__
#define __ENCRYPTER_H__

#include "../utils/matrix.h"

/*
 * Encrypter
 */
void encrypt(const matrix_t image, const matrix_t watermark, int k, int n);

#endif
