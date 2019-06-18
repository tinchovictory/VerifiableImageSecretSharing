#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "encrypter.h"

#define MODULUS 251

/* Helper functions declarations */
static matrix_t buildMatrixA(int m, int k);
static matrix_t* buildVectorsX(int k, int n);
static matrix_t* computeVectorsV(const matrix_t matA, const matrix_t *xArray, int length);
static matrix_t* computeMatricesG(const matrix_t matR, const matrix_t *vArray, int k, int n);
static int getGFromR(const matrix_t matR, int k, int i, int j, int t);
static matrix_t* computeShares(const matrix_t *vArray, const matrix_t *gArray, int length);
static matrix_t* allocMatrixArray(int length);
static void freeMatrixArray(matrix_t *array, int length);



void encrypt(const matrix_t image, const matrix_t watermark, int k, int n) {
  matrix_t matA = NULL, doubleS = NULL, matR = NULL, *xArray = NULL, *vArray = NULL, *gArray = NULL, matRw = NULL, *sharesArray = NULL;

  /* Build matrix A */
  matA = buildMatrixA(get_matrix_height(image), k);
  if(matA == NULL) {
    return;
  }

  /* Calculate matrix projection */
  doubleS = project_matrix(matA);
  if(doubleS == NULL) {
    goto out;
  }

  /* Calculate reminder matrix */
  matR = substract_matrix(image, doubleS);
  if(matR == NULL) {
    goto out;
  }

  /* Choose n of k x 1 random vectors xj */
  xArray = buildVectorsX(k, n);
  if(xArray == NULL) {
    goto out;
  }

  /* Compute vj */
  vArray = computeVectorsV(matA, xArray, n);
  if(vArray == NULL) {
    goto out;
  }

  // freeMatrixArray(xArray, n);

  /* Create shares */
  gArray = computeMatricesG(matR, vArray, k, n);
  if(gArray == NULL) {
    goto out;
  }

  /* Compute watermark remainder */
  matRw = substract_matrix(watermark, doubleS);
  if(matRw == NULL) {
    goto out;
  }

  /* Divide image shares */
  sharesArray = computeShares(vArray, gArray, n);
  if(sharesArray == NULL) {
    goto out;
  }

  // TEST
  printf("Mat A is:\n");
  print_matrix(matA);
  printf("Mat doubleS is:\n");
  print_matrix(doubleS);
  printf("Mat R is:\n");
  print_matrix(matR);
  for(int i = 0; i < n; i++) {
    printf("Mat x%d is:\n", i);
    print_matrix(xArray[i]);
  }
  for(int i = 0; i < n; i++) {
    printf("Mat v%d is:\n", i);
    print_matrix(vArray[i]);
  }
  for(int i = 0; i < n; i++) {
    printf("Mat g%d is:\n", i);
    print_matrix(gArray[i]);
  }
  printf("Mat Rw is:\n");
  print_matrix(matRw);
  for(int i = 0; i < n; i++) {
    printf("Mat Sh%d is:\n", i);
    print_matrix(sharesArray[i]);
  }

  out:
  freeMatrixArray(xArray, n);
  freeMatrixArray(vArray, n);
  freeMatrixArray(gArray, n);
  freeMatrixArray(sharesArray, n);
  free_matrix(matA);
  free_matrix(doubleS);
  free_matrix(matR);
  free_matrix(matRw);
}



/* Helper functions */

/*
 * Build random matrix A
 * Matrix returned should satisfy the 3 conditions:
 *  - rank(A) = k
 *  - (A'A) should be invertible
 */
static matrix_t buildMatrixA(int m, int k) {
  matrix_t matrix;

  matrix = new_matrix(m, k);
  if(matrix == NULL) {
    return NULL;
  }

  /* Set random values */
  set_matrix(matrix, 0, 0, 3); // DEBUG
  set_matrix(matrix, 0, 1, 7); // DEBUG
  set_matrix(matrix, 1, 0, 6); // DEBUG
  set_matrix(matrix, 1, 1, 1); // DEBUG
  set_matrix(matrix, 2, 0, 2); // DEBUG
  set_matrix(matrix, 2, 1, 5); // DEBUG
  set_matrix(matrix, 3, 0, 6); // DEBUG
  set_matrix(matrix, 3, 1, 6); // DEBUG

  return matrix;
}

static matrix_t* buildVectorsX(int k, int n) {
  matrix_t *vectorsArray;

  /* Alloc array */
  vectorsArray = allocMatrixArray(n);
  if(vectorsArray == NULL) {
    return NULL;
  }

  /* Set values to matrix */
  vectorsArray[0] = new_matrix(k, 1); // DEBUG
  set_matrix(vectorsArray[0], 0, 0, 9); // DEBUG
  set_matrix(vectorsArray[0], 1, 0, 5); // DEBUG
  vectorsArray[1] = new_matrix(k, 1); // DEBUG
  set_matrix(vectorsArray[1], 0, 0, 4); // DEBUG
  set_matrix(vectorsArray[1], 1, 0, 4); // DEBUG
  vectorsArray[2] = new_matrix(k, 1); // DEBUG
  set_matrix(vectorsArray[2], 0, 0, 9); // DEBUG
  set_matrix(vectorsArray[2], 1, 0, 8); // DEBUG
  vectorsArray[3] = new_matrix(k, 1); // DEBUG
  set_matrix(vectorsArray[3], 0, 0, 3); // DEBUG
  set_matrix(vectorsArray[3], 1, 0, 2); // DEBUG

  return vectorsArray;
}

static matrix_t* computeVectorsV(const matrix_t matA, const matrix_t *xArray, int length) {
  matrix_t *vArray;

  /* Alloc array */
  vArray = allocMatrixArray(length);
  if(vArray == NULL) {
    return NULL;
  }

  /* Do multiplications */
  for(int i = 0; i < length; i++) {
    vArray[i] = multiply_matrix(matA, xArray[i]);
    if(vArray[i] == NULL) {
      freeMatrixArray(vArray, length);
      return NULL;
    }
  }

  return vArray;
}


static matrix_t* computeMatricesG(const matrix_t matR, const matrix_t *vArray, int k, int n) {
  matrix_t *gArray;
  int g;

  /* Alloc array */
  gArray = allocMatrixArray(n);
  if(gArray == NULL) {
    return NULL;
  }

  /* Build matrices G */
  for(int j = 0; j < n; j++) {
    /* Building Gj */
    gArray[j] = new_matrix(get_matrix_height(matR), (int) ceil((double) get_matrix_width(matR) / k));
    if(gArray[j] == NULL) {
      freeMatrixArray(gArray, n);
      return NULL;
    }

    for(int i = 0; i < get_matrix_height(gArray[j]); i++) {
      /* Iterating over the matrix height */
      for(int t = 0; t < get_matrix_width(gArray[j]); t++) {
        /* Iterating over the matrix width */
        g = getGFromR(matR, k, i, j, t);
        set_matrix(gArray[j], i, t, g);
      }
    }
  }

  return gArray;
}

static int getGFromR(const matrix_t matR, int k, int i, int j, int t) {
  int g = 0, jVal, gVal;
  for(int kVal = 0; kVal < k; kVal++) {
    jVal = k * t + kVal;
    gVal = get_matrix(matR, i, jVal);
    g += gVal * (int) pow((j + 1), kVal);
  }

  return g % MODULUS;
}


static matrix_t* computeShares(const matrix_t *vArray, const matrix_t *gArray, int length) {
  matrix_t *sharesArray;

  /* Alloc array */
  sharesArray = allocMatrixArray(length);
  if(sharesArray == NULL) {
    return NULL;
  }

  for(int i = 0; i < length; i++) {
    sharesArray[i] = concatenate_matrix(vArray[i], gArray[i]);
    if(sharesArray[i] == NULL) {
      freeMatrixArray(sharesArray, length);
      return NULL;
    }
  }

  return sharesArray;
}


static matrix_t* allocMatrixArray(int length) {
  matrix_t *array = NULL;
  array = calloc(length, sizeof(matrix_t));
  return array;
}

static void freeMatrixArray(matrix_t *array, int length) {
  if(array == NULL) {
    return;
  }

  for(int i = 0; i < length; i++) {
    free_matrix(array[i]);
  }
  free(array);
}
