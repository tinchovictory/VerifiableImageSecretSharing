#ifndef __GAUSS_ELIMINATION__
#define __GAUSS_ELIMINATION__

#include "../matrix/matrix.h"

/*
 * Apply gauss-jordan elimination to given matrix
 * It's assumed the matrix is M x N
 * The width param defines the amount of columns to use in the elimination
 * Return NULL on error
 */
matrix_t apply_gauss_elimination(const matrix_t matrix, int width);


#endif
