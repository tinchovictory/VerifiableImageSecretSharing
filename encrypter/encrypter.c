#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "encrypter.h"
#include "../utils/random/random.h"

#define MODULUS 251

/* Helper functions declarations */
static matrix_t buildMatrixA(int m, int k);
static matrix_array_t buildVectorsX(int k, int n);
static matrix_array_t computeVectorsV(const matrix_t matA, const matrix_array_t xArray);
static matrix_array_t computeMatricesG(const matrix_t matR, const matrix_array_t vArray, int k, int n);
static int getGFromR(const matrix_t matR, int k, int i, int j, int t);
static matrix_array_t computeShares(const matrix_array_t vArray, const matrix_array_t gArray);
static int * build_shares_idx(int n);
static int has_value(int *arr, int size, int value);



struct encrypt_output encrypt(const matrix_t image, const matrix_t watermark, int k, int n) {
  matrix_t matA = NULL, doubleS = NULL, matR = NULL, matRw = NULL;
  matrix_array_t xArray = NULL, vArray = NULL, gArray = NULL, sharesArray = NULL;
  struct encrypt_output output = {NULL, NULL, NULL};

  /* Build matrix A */
  matA = buildMatrixA(get_matrix_height(image), k);
  if(matA == NULL) {
    goto out;
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
  vArray = computeVectorsV(matA, xArray);
  if(vArray == NULL) {
    goto out;
  }

  /* Mat A is not used anymore */
  free_matrix(matA);
  matA = NULL;
  /* Array X is not used anymore */
  free_matrix_array(xArray);
  xArray = NULL;

  /* Create shares */
  gArray = computeMatricesG(matR, vArray, k, n);
  if(gArray == NULL) {
    goto out;
  }
  
  /* Mat R is not used anymore */
  free_matrix(matR);
  matR = NULL;

  /* Compute watermark remainder */
  matRw = substract_matrix(watermark, doubleS);
  if(matRw == NULL) {
    goto out;
  }

  /* Matrix double S is not used anymore */
  free_matrix(doubleS);
  doubleS = NULL;

  /* Divide image shares */
  sharesArray = computeShares(vArray, gArray);
  if(sharesArray == NULL) {
    goto out;
  }

  /* Fill output structure */
  output.shares = sharesArray;
  output.sharesIdx = build_shares_idx(n);
  output.remainder = matRw;

  out:
  free_matrix_array(xArray);
  free_matrix_array(vArray);
  free_matrix_array(gArray);
  free_matrix(matA);
  free_matrix(doubleS);
  free_matrix(matR);

  return output;
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

static matrix_array_t buildVectorsX(int k, int n) {
  matrix_array_t xArray;
  matrix_t matX;
  int *set, rand;

  /* Alloc set */
  set = (int *) calloc(n, sizeof(int));
  if(set == NULL) {
    return NULL;
  }

  /* Alloc array */
  xArray = new_matrix_array(n);
  if(xArray == NULL) {
    free(set);
    return NULL;
  }

  /* Build all the matrix inside the array */
  for(int p = 0; p < n; p++) {
    /* Alloc matrix */
    matX = new_matrix(k, 1);
    if(matX == NULL) {
      free(set);
      free_matrix_array(xArray);
      return NULL;
    }

    /* Get random value not in set */
    do {
      rand = next_char() % MODULUS;
    } while(has_value(set, n, rand));
    set[p] = rand;

    /* Fill matrix */
    for(int i = 0; i < k; i++) {
      set_matrix(matX, i, 0, (int) pow(rand, i));
    }

    /* Add matrix to the array */
    add_matrix_array(xArray, matX, p);
  }

  free(set);

  return xArray;
}

static matrix_array_t computeVectorsV(const matrix_t matA, const matrix_array_t xArray) {
  matrix_array_t vArray;
  matrix_t vMatrix;

  /* Alloc array */
  vArray = new_matrix_array(get_matrix_array_size(xArray));
  if(vArray == NULL) {
    return NULL;
  }

  /* Do multiplications */
  for(int i = 0; i < get_matrix_array_size(xArray); i++) {
    vMatrix = multiply_matrix(matA, get_matrix_array_item(xArray, i));
    if(vMatrix == NULL) {
      free_matrix_array(vArray);
      return NULL;
    }

    add_matrix_array(vArray, vMatrix, i);
  }

  return vArray;
}


static matrix_array_t computeMatricesG(const matrix_t matR, const matrix_array_t vArray, int k, int n) {
  matrix_array_t gArray;
  matrix_t gMatrix;
  int g;

  /* Alloc array */
  gArray = new_matrix_array(n);
  if(gArray == NULL) {
    return NULL;
  }

  /* Build matrices G */
  for(int j = 0; j < n; j++) {
    /* Building Gj */
    gMatrix = new_matrix(get_matrix_height(matR), (int) ceil((double) get_matrix_width(matR) / k));
    if(gMatrix == NULL) {
      free_matrix_array(gArray);
      return NULL;
    }

    add_matrix_array(gArray, gMatrix, j);

    for(int i = 0; i < get_matrix_height(gMatrix); i++) {
      /* Iterating over the matrix height */
      for(int t = 0; t < get_matrix_width(gMatrix); t++) {
        /* Iterating over the matrix width */
        g = getGFromR(matR, k, i, j, t);
        set_matrix(gMatrix, i, t, g);
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


static matrix_array_t computeShares(const matrix_array_t vArray, const matrix_array_t gArray) {
  matrix_array_t sharesArray;
  matrix_t share, vMatrix, gMatrix;

  /* Alloc array */
  sharesArray = new_matrix_array(get_matrix_array_size(gArray));
  if(sharesArray == NULL) {
    return NULL;
  }

  for(int i = 0; i < get_matrix_array_size(sharesArray); i++) {
    vMatrix = get_matrix_array_item(vArray, i);
    gMatrix = get_matrix_array_item(gArray, i);
    share = concatenate_matrix(vMatrix, gMatrix);

    if(share == NULL) {
      free_matrix_array(sharesArray);
      return NULL;
    }

    add_matrix_array(sharesArray, share, i);
  }

  return sharesArray;
}

static int * build_shares_idx(int n) {
  int * array = (int *) malloc(n * sizeof(int));
  if(array == NULL) {
    return NULL;
  }

  for(int i = 0; i < n; i++) {
    array[i] = i + 1;
  }

  return array;
}


static int has_value(int *arr, int size, int value) {
  if(arr == NULL) {
    return 0;
  }

  for(int i = 0; i < size; i++) {
    if(arr[i] == value) {
      return 1;
    }
  }

  return 0;
}
