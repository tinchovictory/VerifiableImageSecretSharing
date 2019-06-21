#include <stdio.h>
#include <stdlib.h>

#include "decrypter.h"
#include "../utils/gaussElimination/gaussElimination.h"

/* Helper functions definitions */
static matrix_t build_matrix_B(const matrix_array_t shares);
static int get_shares_height(const matrix_array_t shares);
static matrix_array_t build_matrices_G(const matrix_array_t shares);
static matrix_t build_matrix_R(const int sharesIdx[], const matrix_array_t arrayG);

/*
 * Decrypter
 */
void decrypt(const matrix_array_t shares, const int sharesIdx[], const matrix_t remainder) {
  matrix_t matB = NULL, doubleS = NULL, matR = NULL, matS = NULL, matW = NULL;
  matrix_array_t arrayG = NULL;

  /* Build matrix B */
  matB = build_matrix_B(shares);
  if(matB == NULL) {
    printf("failed mat b\n");
    goto out;
  }

  /* Compute matrix double S */
  doubleS = project_matrix(matB);
  if(doubleS == NULL) {
    printf("failed mat s\n");
    goto out;
  }

  // Free matB

  /* Build matrices G */
  arrayG = build_matrices_G(shares);
  if(arrayG == NULL) {
    printf("failed mat g\n");
    goto out;
  }

  /* Build remainder matrix R */
  matR = build_matrix_R(sharesIdx, arrayG);
  if(matR == NULL) {
    goto out;
  }

  /* Build secret matrix S */
  matS = add_matrix(doubleS, matR);
  if(matS == NULL) {
    goto out;
  }

  /* Build watermark */
  matW = add_matrix(doubleS, remainder);
  if(matW == NULL) {
    goto out;
  }


  // TEST
  printf("Mat B is \n");
  print_matrix(matB);
  printf("Mat double S is \n");
  print_matrix(doubleS);
  print_matrix_array(arrayG, "G");
  printf("Mat R is \n");
  print_matrix(matR);
  printf("Mat S is \n");
  print_matrix(matS);
  printf("Mat W is \n");
  print_matrix(matW);

  out:
  free_matrix(matB);
  free_matrix(doubleS);
  free_matrix_array(arrayG);
  free_matrix(matR);
  free_matrix(matS);
  free_matrix(matW);
}



/* Helper functions */

static matrix_t build_matrix_B(const matrix_array_t shares) {
  matrix_t matB, share;

  /* Allocate matB of size m x k */
  matB = new_matrix(get_shares_height(shares), get_matrix_array_size(shares));
  if(matB == NULL) {
    return NULL;
  }

  for(int j = 0; j < get_matrix_array_size(shares); j++) {
    share = get_matrix_array_item(shares, j);
    if(share == NULL) {
      free_matrix(matB);
      return NULL;
    }

    for(int i = 0; i < get_matrix_height(share); i++) {
      set_matrix(matB, i, j, get_matrix(share, i, 0));
    }
  }

  return matB;
}

static int get_shares_height(const matrix_array_t shares) {
  matrix_t share;

  if(shares == NULL) {
    return -1;
  }

  share = get_matrix_array_item(shares, 0);
  if(share == NULL) {
    return -1;
  }

  return get_matrix_height(share);
}


static matrix_array_t build_matrices_G(const matrix_array_t shares) {
  matrix_array_t arrayG;
  matrix_t share, matG;

  /* Allocate array G */
  arrayG = new_matrix_array(get_matrix_array_size(shares));
  if(arrayG == NULL) {
    return NULL;
  }

  /* For each share create a G matrix */
  for(int sh = 0; sh < get_matrix_array_size(shares); sh++) {
    /* Get current share */
    share = get_matrix_array_item(shares, sh);
    if(share == NULL) {
      free_matrix_array(arrayG);
      return NULL;
    }

    /* Create G matrix */
    matG = new_matrix(get_matrix_height(share), get_matrix_width(share) - 1);
    if(matG == NULL) {
      free_matrix_array(arrayG);
      return NULL;
    }

    /* Copy share values to G */
    for(int i = 0; i < get_matrix_height(matG); i++) {
      for(int j = 0; j < get_matrix_width(matG); j++) {
        set_matrix(matG, i, j, get_matrix(share, i, j + 1));
      }
    }

    /* Add G matrix to array */
    add_matrix_array(arrayG, matG, sh);
  }

  return arrayG;
}


static matrix_t build_matrix_R(const int sharesIdx[], const matrix_array_t arrayG) {
  matrix_t matR, fShare, matEq, share, solvedEq;
  int Rj;

  /* Get first share to read dimensions */
  fShare = get_matrix_array_item(arrayG, 0);
  if(fShare == NULL) {
    return NULL;
  }

  /* Allocate matR */
  matR = new_matrix(get_matrix_height(fShare), get_matrix_array_size(arrayG) * get_matrix_width(fShare));
  if(matR == NULL) {
    return NULL;
  }

  /* Build equation matrix with the shares index */
  matEq = new_matrix(get_matrix_array_size(arrayG), 3);
  if(matEq == NULL) {
    free_matrix(matR);
    return NULL;
  }

  /* Fill equation matrix with shares index */
  for(int i = 0; i < get_matrix_height(matEq); i++) {
    set_matrix(matEq, i, 0, 1);
    set_matrix(matEq, i, 1, sharesIdx[i]);
  }

  /* Iterate over the shares to fill R */
  for(int i = 0; i < get_matrix_height(fShare); i++) {
    for(int j = 0; j < get_matrix_width(fShare); j++) {
      /* Iterate over the shares array */
      for(int k = 0; k < get_matrix_array_size(arrayG); k++) {
        share = get_matrix_array_item(arrayG, k);
        set_matrix(matEq, k, 2, get_matrix(share, i, j));
      }

      /* Calculate gauss jordan elimination */
      solvedEq = apply_gauss_elimination(matEq);
      if(solvedEq == NULL) {
        free_matrix(matR);
        free_matrix(matEq);
        return NULL;
      }

      /* Fill values to R */
      for(int k = 0; k < get_matrix_height(solvedEq); k++) {
        Rj = j * get_matrix_height(solvedEq) + k;
        set_matrix(matR, i, Rj, get_matrix(solvedEq, k, 2));
      }

      free_matrix(solvedEq);
    }
  }

  free_matrix(matEq);

  return matR;
}