#include <stdio.h>
#include <stdlib.h>

#include "decrypter.h"

/* Helper functions definitions */
// static int is_shares_array_valid(const matrix_array_t shares, int k);
static matrix_t build_matrix_B(const matrix_array_t shares);
static int get_shares_height(const matrix_array_t shares);
static matrix_array_t build_matrices_G(const matrix_array_t shares);

/*
 * Decrypter
 */
void decrypt(const matrix_array_t shares, const int sharesIdx[], const matrix_t remainder) {
  matrix_t matB = NULL, doubleS = NULL;
  matrix_array_t arrayG = NULL;

  /* Check shares size */
  // if(!is_shares_array_valid(shares, k)) {
  //   goto out;
  // }

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

  /* Build matrices G */
  arrayG = build_matrices_G(shares);
  if(arrayG == NULL) {
    printf("failed mat g\n");
    goto out;
  }

  /* Build remainder matrix R */

  /* Build secret matrix S */

  /* Build watermark */


  // TEST
  printf("Mat B is \n");
  print_matrix(matB);
  printf("Mat double S is \n");
  print_matrix(doubleS);
  print_matrix_array(arrayG, "G");

  out:
  free_matrix(matB);
  free_matrix(doubleS);
  free_matrix_array(arrayG);
}


/* Helper functions */

// static int is_shares_array_valid(const matrix_array_t shares, int k) {
//   int count = 0;

//   if(shares == NULL) {
//     return 0;
//   }

//   for(int i = 0; i < get_matrix_array_size(shares); i++) {
//     if(get_matrix_array_item(shares, i) != NULL) {
//       count++;
//     }
//   }

//   return count == k;
// }


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
