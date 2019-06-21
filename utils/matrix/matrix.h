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
 * Add matrixA to matrixB
 * matrixA + matrixB
 * Return new matrix with the operation
 * Return NULL on error
 */
matrix_t add_matrix(const matrix_t matrixA, const matrix_t matrixB);

/*
 * Project matrix
 * Projection is (A x ( (A' x A) ^-1 ) x A')
 * Return NULL on error
 */
matrix_t project_matrix(const matrix_t matrix);

/*
 * Concatenate matrixA | matrixB
 * Return new matrix with new size and values
 * Return NULL on error
 */
matrix_t concatenate_matrix(const matrix_t matrixA, const matrix_t matrixB);

/*
 * Getter of the value in the position (i, j)
 * If position is not valid return -1
 */
int get_matrix(const matrix_t matrix, int i, int j);

/*
 * Set value to matrix in position (i, j)
 * Returns 1 on success, 0 on error
 */
int set_matrix(matrix_t matrix, int i, int j, int value);

/*
 * Matrix height getter
 * On error return -1
 */
int get_matrix_height(const matrix_t matrix);

/*
 * Matrix width getter
 * On error return -1
 */
int get_matrix_width(const matrix_t matrix);

/*
 * Print matrix
 */
void print_matrix(const matrix_t matrix);

#endif
