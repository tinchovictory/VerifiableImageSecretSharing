#ifndef __MATRIX_UTILS__
#define __MATRIX_UTILS__


typedef struct matrix * matrix_t;

/*
 * Init a new matrix of size width x length
 * If dimesions are invalid returns NULL
 * If unable to allocate memory returns NULL
 */
matrix_t new_matrix(int height, int width);

/*
 * Free up a matrix
 */
void free_matrix(matrix_t matrix);

/*
 * Clone param matrix
 */
matrix_t clone_matrix(const matrix_t matrix);

/*
 * Transpose param matrix
 * Return new matrix transposed. Original matrix is not free up
 */
matrix_t transpose_matrix(const matrix_t matrix);

/*
 * Inverse param matrix in place
 * If matrix is not invertible return 0
 * On success return 1
 */
int inverse_matrix(matrix_t matrix);

/*
 * Multiply param matrixA with param matrixB
 * Return multiplied matrix
 */
matrix_t multiply_matrix(const matrix_t matrixA, const matrix_t matrixB);


/*
 *
 */
int set_matrix(matrix_t matrix, int i, int j, int value);

/*
 *
 */
void print_matrix(matrix_t matrix);

/* MISSING
 *  - Inverse matrix
 *  - Substract matrix
 *  - Project matrix
 *  - Apply modulus
 */

#endif