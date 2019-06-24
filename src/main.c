#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include "paramParser/paramParser.h"

#include "encrypter/encrypter.h"
#include "decrypter/decrypter.h"
#include "bmpReader/bmpReader.h"
#include "stenography/stenography.h"

#include "utils/matrix/matrix.h"
#include "utils/matrixArray/matrixArray.h"
#include "utils/random/random.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

static int encrypt_image(struct params params);
static int decrypt_image(struct params params);
static image_t * shares_array(const char *directory, int amount);
static void free_shares_array(image_t *array, int size);

int main(int argc, char *argv[]) {
  int err = 1;

  set_seed(10);

  /* Read params */
  struct params params = parse_params(argc, argv);
  if(params.action == ACTION_ERROR) {
    return EXIT_FAILURE;
  } 

  /* Do encryption or decryption */
  if(params.action == ACTION_ENCRYPT) {
    printf("%s[INFO] Encryption started%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    err = encrypt_image(params);
  } else if(params.action == ACTION_DECRYPT) {
    printf("%s[INFO] Decryption started%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    err = decrypt_image(params);
  }

  /* Dealloc memory */
  free_params(params);

  if(err) {
    return EXIT_FAILURE;
  }

  printf("%s[INFO] %s finished successfully%s\n", ANSI_COLOR_GREEN, params.action == ACTION_ENCRYPT ? "Encryption" : "Decryptions", ANSI_COLOR_RESET);
  return EXIT_SUCCESS;
}

/*
 * Returns 0 on success
 */
static int encrypt_image(struct params params) {
  image_t *sharesArray = NULL, secret = NULL, watermark = NULL, rw = NULL;
  matrix_t matS = NULL, matW = NULL, share;
  int err = 0, pixel = 0, sharesOffset = 0, pixelRw = 0, byte;
  unsigned char *pixelsArray = NULL;

  /* Open image shares */
  sharesArray = shares_array(params.dir, params.n);
  if(sharesArray == NULL) {
    err = 1;
    goto err;
  }
  for(int i = 0; i < params.n; i++) {
    set_image_reserved(sharesArray[i], i);
  }

  /* Open secret images */
  secret = open_image(params.secretImage);
  if(secret == NULL) {
    printf("%s[ERROR] Unable to open secret image%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    err = 1;
    goto err;
  }
  
  /* Open watermark */
  watermark = open_image(params.watermark);
  if(watermark == NULL) {
    printf("%s[ERROR] Unable to open watermark image\n%s", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    err = 1;
    goto err;
  }

  /* Matrix Alloc */
  matS = new_matrix(params.n, params.n);
  matW = new_matrix(params.n, params.n);
  if(matS == NULL || matW == NULL) {
    printf("%s[ERROR] Out of memory%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    err = 1;
    goto err;
  }
  
  /* Create remainder image */ 
  rw = new_8bit_image(params.rwFile, get_image_width(sharesArray[0]), get_image_height(sharesArray[0]));
  if(rw == NULL) {
    printf("%s[ERROR] Unable to create image RW%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    err = 1;
    goto err;
  }

  /* Alloc pixels array */
  pixelsArray = malloc(params.n * sizeof(unsigned char));
  if(pixelsArray == NULL) {
    printf("%s[ERROR] Out of memory%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    err = 1;
    goto err;
  }


  /* Iterate over the image */
  while(pixel < get_image_height(sharesArray[0]) * get_image_width(sharesArray[0])) {
    /* Build matrix S and R */
    for(int i = 0; i < params.n; i++) {
      for(int j = 0; j < params.n; j++) {
        set_matrix(matS, i, j, get_image_pixel(secret, pixel));
        set_matrix(matW, i, j, get_image_pixel(watermark, pixel));
        pixel++;
      }
    }

    /* Encrypt */
    struct encrypt_output output = encrypt(matS, matW, params.k, params.n);
    if(output.shares == NULL || output.remainder == NULL) {
      printf("%s[ERROR] Unable to encrypt%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
      free_encrypt_output(output);
      err = 1;
      goto err;
    }

    /* Hide the encrypted values in the shares */
    for(int i = 0; i < params.n; i++) {
      for(int j = 0; j < (params.n / params.k) + 1; j++) {
        for(int k = 0; k < get_matrix_array_size(output.shares); k++) {
          share = get_matrix_array_item(output.shares, k);
          byte = get_matrix(share, i, j);

          /* Read the next n bytes from the share */
          for(int p = 0; p < params.n; p++) {
            pixelsArray[p] = get_image_pixel(sharesArray[k], sharesOffset + p);
          }

          /* Hide the byte */
          stenography_hide(pixelsArray, params.n, byte, 8 / params.n);

          /* Set the pixels to the image share again */
          for(int p = 0; p < params.n; p++) {
            set_image_pixel(sharesArray[k], pixelsArray[p], sharesOffset + p);
          }
        }
        sharesOffset += params.n;
      }
    }

    /* Fill the rw image */    
    for(int i = 0; i < params.n; i++) {
      for(int j = 0; j < params.n; j++) {
        set_image_pixel(rw, get_matrix(output.remainder, i, j), pixelRw++);
      }
    }

    /* Dealloc output */
    free_encrypt_output(output);
  }

  /* Handle errors */

  err:

  free_shares_array(sharesArray, params.n);
  close_image(secret);
  close_image(watermark);
  close_image(rw);

  free_matrix(matS);
  free_matrix(matW);

  free(pixelsArray);
  
  return err;
}

/*
 * Returns 0 on success
 */
static int decrypt_image(struct params params) {
  image_t *sharesArray = NULL, secret = NULL, watermark = NULL, rw = NULL;
  matrix_array_t matSharesArray = NULL;
  matrix_t share, remainder = NULL;
  int *shareIdx = NULL;
  int err = 0, pixel = 0, pixelOut = 0, pixelRw = 0;
  unsigned char *pixelsArray = NULL;

  /* Open image shares */
  sharesArray = shares_array(params.dir, params.k);
  if(sharesArray == NULL) {
    err = 1;
    goto err;
  }

  /* Get the share idxs */
  shareIdx = malloc(params.k);
  if(shareIdx == NULL) {
    printf("%s[ERROR] Out of memory%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    err = 1;
    goto err;
  }
  for(int i = 0; i < params.k; i++) {
    shareIdx[i] = get_image_reserved(sharesArray[i]);
  }

  /* Open remainder images */ 
  rw = open_image(params.rwFile);
  if(rw == NULL) {
    printf("%s[ERROR] Unable to open image RW%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    err = 1;
    goto err;
  }

  /* Build matrix shares array */
  matSharesArray = new_matrix_array(params.k);
  if(matSharesArray == NULL) {
    printf("%s[ERROR] Out of memory%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    err = 1;
    goto err;
  }
  for(int i = 0; i < get_matrix_array_size(matSharesArray); i++) {
    share = new_matrix(params.n, params.n / params.k + 1);
    if(share == NULL) {
      printf("%s[ERROR] Out of memory%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
      err = 1;
      goto err;
    }
    add_matrix_array(matSharesArray, share, i);
  }

  /* Build remainder matrix */
  remainder = new_matrix(params.n, params.n);
  if(remainder == NULL) {
    printf("%s[ERROR] Out of memory%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    err = 1;
    goto err;
  }
  
  /* Alloc pixels array */
  pixelsArray = malloc(params.n * sizeof(unsigned char));
  if(pixelsArray == NULL) {
    printf("%s[ERROR] Out of memory%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    err = 1;
    goto err;
  }

  /* Create images */
  secret = new_8bit_image(params.secretImage, get_image_width(rw), get_image_height(rw));
  watermark = new_8bit_image(params.watermark, get_image_width(rw), get_image_height(rw));
  if(secret == NULL || watermark == NULL) {
    printf("%s[ERROR] Unable to create output images%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    err = 1;
    goto err;
  }

  while(pixel < get_image_height(sharesArray[0]) * get_image_width(sharesArray[0]) * 3) {
    for(int i = 0; i < params.n; i++) {
      for(int j = 0; j < (params.n / params.k) + 1; j++) {
        for(int k = 0; k < get_matrix_array_size(matSharesArray); k++) {

          /* Read n pixels to do the stenography  */
          for(int p = 0; p < params.n; p++) {
            pixelsArray[p] = get_image_pixel(sharesArray[k], pixel + p);
          }

          /* Get the stenography */
          unsigned char hiddenValue = stenography_get(pixelsArray, params.n, 8 / params.n);

          /* Set the hidden value in the current position of the matrix */
          share = get_matrix_array_item(matSharesArray, k);
          set_matrix(share, i, j, hiddenValue);
        }

        /* Advance the pixels */
        pixel += params.n;
      }
    }

    for(int i = 0; i < params.n; i++) {
      for(int j = 0; j < params.n; j++) {
        /* Fill the remainder matrix */
        set_matrix(remainder, i, j, get_image_pixel(rw, pixelRw++));
      }
    }

    /* Decrypt */
    struct decrypt_output output = decrypt(matSharesArray, shareIdx, remainder);

    if(output.secret == NULL || output.watermark == NULL) {
      free_decrypt_output(output);
      return 1;
    }

    for(int i = 0; i < params.n; i++) {
      for(int j = 0; j < params.n; j++) {
        set_image_pixel(secret, get_matrix(output.secret, i, j), pixelOut);
        set_image_pixel(watermark, get_matrix(output.watermark, i, j), pixelOut);
        pixelOut++;
      }
    }

    free_decrypt_output(output);
  }


  err:

  free_shares_array(sharesArray, params.k);
  free(shareIdx);
  close_image(rw);
  free_matrix_array(matSharesArray);
  free_matrix(remainder);
  free(pixelsArray);
  close_image(secret);
  close_image(watermark);

  return err;
}

/*
 * Open the images from the directory
 * Return NULL on error
 */
static image_t * shares_array(const char *directory, int amount) {
  image_t *array;
  DIR *dir;
  struct dirent *dirent;
  struct stat statbuf;
  int counter = 0;
  char buffer[1000] = {0};

  /* Alloc array */
  array = (image_t *) calloc(amount, sizeof(image_t));
  if(array == NULL) {
    return NULL;
  }

  /* Open directory */
  dir = opendir(directory);
  if(dir == NULL) {
    printf("%s[ERROR] Unable to open directory: %s%s\n", ANSI_COLOR_RED, directory, ANSI_COLOR_RESET);
    free_shares_array(array, amount);
    return NULL;
  }

  /* Open amount files of the directory */
  while(counter < amount && (dirent = readdir(dir)) != NULL) {
    memset(&statbuf, 0, sizeof(statbuf));
    lstat(dirent->d_name, &statbuf);
    if(strncmp(".", dirent->d_name, 1) != 0 && strncmp("..", dirent->d_name, 2) != 0 && !S_ISDIR(statbuf.st_mode)) {
      strcpy(buffer, directory);
      strcat(buffer, dirent->d_name);
      array[counter] = open_image(buffer);
      if(array[counter] == NULL) {
        break;
      }
      counter++;
    }
  }

  /* Check if all files are opened */
  if(counter < amount) {
    printf("%s[ERROR] Unable to open at least %d shares%s\n", ANSI_COLOR_RED, amount, ANSI_COLOR_RESET);
    free_shares_array(array, amount);
    array = NULL;
  }

  closedir(dir);

  return array;
}

static void free_shares_array(image_t *array, int size) {
  if(array == NULL) {
    return;
  }

  for(int i = 0; i < size; i++) {
    close_image(array[i]);
  }

  free(array);
}
