#include <stdio.h>

#include "../utils/matrix/matrix.h"
#include "../utils/matrixArray/matrixArray.h"
#include "decrypter.h"

static matrix_array_t readShares();
static matrix_t readReminder();

int main(void) {
  matrix_t reminder = readReminder();
  matrix_array_t shares = readShares();
  int sharesIdx[] = {1, 2};

  if(reminder == NULL || shares == NULL) {
    return 1;
  }

  decrypt(shares, sharesIdx, reminder);

  return 0;
}


static matrix_array_t readShares() {
  matrix_t share;
  matrix_array_t shares = new_matrix_array(2);
  if(shares == NULL) {
    return NULL;
  }

  share = new_matrix(4, 3);
  if(share == NULL) {
    free_matrix_array(shares);
    return NULL;
  }

  set_matrix(share, 0, 0, 62);
  set_matrix(share, 0, 1, 36);
  set_matrix(share, 0, 2, 92);
  set_matrix(share, 1, 0, 59);
  set_matrix(share, 1, 1, 101);
  set_matrix(share, 1, 2, 58);
  set_matrix(share, 2, 0, 43);
  set_matrix(share, 2, 1, 126);
  set_matrix(share, 2, 2, 142);
  set_matrix(share, 3, 0, 84);
  set_matrix(share, 3, 1, 26);
  set_matrix(share, 3, 2, 163);

  add_matrix_array(shares, share, 0);


  share = new_matrix(4, 3);
  if(share == NULL) {
    free_matrix_array(shares);
    return NULL;
  }

  set_matrix(share, 0, 0, 40);
  set_matrix(share, 0, 1, 71);
  set_matrix(share, 0, 2, 132);
  set_matrix(share, 1, 0, 28);
  set_matrix(share, 1, 1, 169);
  set_matrix(share, 1, 2, 47);
  set_matrix(share, 2, 0, 28);
  set_matrix(share, 2, 1, 198);
  set_matrix(share, 2, 2, 72);
  set_matrix(share, 3, 0, 48);
  set_matrix(share, 3, 1, 14);
  set_matrix(share, 3, 2, 150);

  add_matrix_array(shares, share, 1);

  return shares;
}

static matrix_t readReminder() {
  matrix_t matrix = new_matrix(4, 4);
  if(matrix == NULL) {
    return NULL;
  }

  set_matrix(matrix, 0, 0, 49);
  set_matrix(matrix, 0, 1, 130);
  set_matrix(matrix, 0, 2, 71);
  set_matrix(matrix, 0, 3, 51);
  set_matrix(matrix, 1, 0, 52);
  set_matrix(matrix, 1, 1, 138);
  set_matrix(matrix, 1, 2, 14);
  set_matrix(matrix, 1, 3, 38);
  set_matrix(matrix, 2, 0, 51);
  set_matrix(matrix, 2, 1, 156);
  set_matrix(matrix, 2, 2, 2);
  set_matrix(matrix, 2, 3, 182);
  set_matrix(matrix, 3, 0, 61);
  set_matrix(matrix, 3, 1, 50);
  set_matrix(matrix, 3, 2, 20);
  set_matrix(matrix, 3, 3, 100);

  return matrix;
}
