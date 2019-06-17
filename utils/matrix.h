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
 * Inverse param matrix
 * If matrix is not invertible return NULL
 */
matrix_t inverse_matrix(const matrix_t matrix);

/*
 * Multiply param matrixA with param matrixB
 * Return multiplied matrix
 */
matrix_t multiply_matrix(const matrix_t matrixA, const matrix_t matrixB);

/*
 * Determinant of matrix
 * If out of memory error return -1
 * Otherwise return the determinant of the matrix
 */
int determinant_of_matrix(const matrix_t matrix);

/*
 * Substract matrixB from matrixA
 * matrixA - matrixB
 * Return new matrix with the operation
 * Return NULL on error
 */
matrix_t substract_matrix(const matrix_t matrixA, const matrix_t matrixB);

/*
 * Project matrix
 * Projection is (A x ( (A' x A) ^-1 ) x A')
 * Return NULL on error
 */
matrix_t project_matrix(const matrix_t matrix);


/*
 *
 */
int set_matrix(matrix_t matrix, int i, int j, int value);

/*
 *
 */
void print_matrix(matrix_t matrix);

/* MISSING
 *  - Apply modulus
 */

#endif
