#ifndef __GAUSS_ELIMINATION__
#define __GAUSS_ELIMINATION__

#include "../matrix/matrix.h"

/*
 * Apply gauss-jordan elimination to given matrix in place
 * It's assumed the matrix is M x N, with N = ( M + 1 )
 * The last column of the matrix represent the result values
 * Return 0 on error
 * Return 1 on success
 */
int apply_gauss_elimination(matrix_t matrix);


#endif
