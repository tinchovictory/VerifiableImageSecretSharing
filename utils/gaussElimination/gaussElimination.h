#ifndef __GAUSS_ELIMINATION__
#define __GAUSS_ELIMINATION__

#include "../matrix/matrix.h"

/*
 * Apply gauss-jordan elimination to given matrix
 * It's assumed the matrix is M x N, with N = ( M + 1 )
 * The last column of the matrix represent the result values
 * Return NULL on error
 */
matrix_t apply_gauss_elimination(const matrix_t matrix);


#endif
