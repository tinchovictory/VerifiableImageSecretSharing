#include <stdio.h>

#include "encrypter/encrypter.h"
#include "bmpReader/bmpReader.h"
#include "stenography/stenography.h"

#include "utils/matrix/matrix.h"
#include "utils/matrixArray/matrixArray.h"
#include "utils/random/random.h"

#define K 4
#define N 8
#define BITS_PER_PIXEL 1


int main(void) {
  set_seed(10);

  /* Open 4 shares images */
  image_t share1 = open_image("shares/share1.bmp");
  image_t share2 = open_image("shares/share2.bmp");
  image_t share3 = open_image("shares/share3.bmp");
  image_t share4 = open_image("shares/share4.bmp");
  image_t share5 = open_image("shares/share5.bmp");
  image_t share6 = open_image("shares/share6.bmp");
  image_t share7 = open_image("shares/share7.bmp");
  image_t share8 = open_image("shares/share8.bmp");

  image_t sharesArray[] = {share1, share2, share3, share4, share5, share6, share7, share8};
  // image_t sharesArray[] = {share1, share2, share3, share4};

  /* Open remainder images */ 
  image_t rw = new_8bit_image("RW.bmp", 280, 440);

  /* Create output images */
  image_t secret = open_image("secret.bmp");
  image_t watermark = open_image("watermarkOriginal.bmp");

  /* Matrix */
  matrix_t matS = new_matrix(N, N);
  matrix_t matW = new_matrix(N, N);

  /* Iterate over the image */
  int pixel = 0;
  int sharesOffset = 0;
  int pixelRw = 0;
  while(pixel < get_image_height(share1) * get_image_width(share1)) {
    /* Build matrix S and R */
    for(int i = 0; i < N; i++) {
      for(int j = 0; j < N; j++) {
        set_matrix(matS, i, j, get_image_pixel(secret, pixel));
        set_matrix(matW, i, j, get_image_pixel(watermark, pixel));
        pixel++;
      }
    }

    /* Encrypt */
    struct encrypt_output output = encrypt(matS, matW, K, N);
    if(output.shares == NULL || output.remainder == NULL) {
      printf("Unable to encrypt\n");
      return 1;
    }

    for(int i = 0; i < N; i++) {
      for(int j = 0; j < 3; j++) {
        for(int k = 0; k < get_matrix_array_size(output.shares); k++) {
          matrix_t share = get_matrix_array_item(output.shares, k);
          int byte = get_matrix(share, i, j);

          /* Read the next n bytes from the share */
          unsigned char pixelsArray[N];
          for(int p = 0; p < N; p++) {
            pixelsArray[p] = get_image_pixel(sharesArray[k], sharesOffset + p);
          }

          /* Hide the byte */
          stenography_hide(pixelsArray, N, byte, BITS_PER_PIXEL);

          /* Set the pixels to the image share again */
          for(int p = 0; p < N; p++) {
            set_image_pixel(sharesArray[k], pixelsArray[p], sharesOffset + p);
          }
        }
        sharesOffset += N;
      }
    }

    /* Fill the rw image */    
    for(int i = 0; i < N; i++) {
      for(int j = 0; j < N; j++) {
        set_image_pixel(rw, get_matrix(output.remainder, i, j), pixelRw++);
      }
    }
  }

  close_image(share1);
  close_image(share2);
  close_image(share3);
  close_image(share4);
  close_image(share5);
  close_image(share6);
  close_image(share7);
  close_image(share8);
  close_image(rw);

  close_image(secret);
  close_image(watermark);

  free_matrix(matS);
  free_matrix(matW);

  return 0;
}
