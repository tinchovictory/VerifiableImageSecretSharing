#include <stdio.h>

#include "decrypter/decrypter.h"
#include "bmpReader/bmpReader.h"
#include "stenography/stenography.h"

#include "utils/matrix/matrix.h"
#include "utils/matrixArray/matrixArray.h"

#define K 2
#define N 4
#define BITS_PER_PIXEL 2


int main(void) {
  /* Open 4 shares images */
  image_t share1 = open_image("grupo8/2-4/share1.bmp");
  image_t share2 = open_image("grupo8/2-4/share2.bmp");
  // image_t share3 = open_image("grupo8/2-4/share3.bmp");
  // image_t share4 = open_image("grupo8/2-4/share4.bmp");

  // image_t sharesImg[] = {share1, share2, share3, share4};
  image_t sharesImg[] = {share1, share2};

  /* Get the share idx */
  int shareIdx[K];
  shareIdx[0] = get_image_reserved(share1);
  shareIdx[1] = get_image_reserved(share2);
  // shareIdx[2] = get_image_reserved(share3);
  // shareIdx[3] = get_image_reserved(share4);

  /* Open remainder images */ 
  image_t rw = open_image("grupo8/2-4/RW.bmp");

  /* Build shares array */
  matrix_array_t sharesArray = new_matrix_array(K);
  for(int i = 0; i < get_matrix_array_size(sharesArray); i++) {
    add_matrix_array(sharesArray, new_matrix(N, 3), i);
  }

  /* Build the remainder matrix */
  matrix_t remainder = new_matrix(N, N);

  /* Create output images */
  image_t secret = new_8bit_image("secretImage.bmp", get_image_width(rw), get_image_height(rw));
  image_t watermark = new_8bit_image("watermark.bmp", get_image_width(rw), get_image_height(rw));

  /* Iterate over the image */
  int pixel = 0;
  int pixelRw = 0;
  int pixelOut = 0;
  while(pixel < get_image_height(share1) * get_image_width(share1) * 3) {
    for(int i = 0; i < N; i++) {
      for(int j = 0; j < 3; j++) {
        for(int k = 0; k < get_matrix_array_size(sharesArray); k++) {

          /* Read n pixels to do the stenography  */
          unsigned char pixelsArray[N];
          for(int p = 0; p < N; p++) {
            pixelsArray[p] = get_image_pixel(sharesImg[k], pixel + p);
            // printf("pixel: %d\n", pixelsArray[p]);
          }

          /* Get the stenography */
          unsigned char hiddenValue = stenography_get(pixelsArray, N, BITS_PER_PIXEL);
          // printf("hidden value: %d\n", hiddenValue);

          /* Set the hidden value in the current position of the matrix */
          matrix_t share = get_matrix_array_item(sharesArray, k);
          set_matrix(share, i, j, hiddenValue);
        }

        /* Advance the pixels */
        pixel += N;
      }
    }

    for(int i = 0; i < N; i++) {
      for(int j = 0; j < N; j++) {
        /* Fill the remainder matrix */
        set_matrix(remainder, i, j, get_image_pixel(rw, pixelRw++));
      }
    }
    // print_matrix_array(sharesArray, "share");
    // print_matrix(remainder);

    /* Decrypt */
    struct decrypt_output output = decrypt(sharesArray, shareIdx, remainder);

    if(output.secret == NULL || output.watermark == NULL) {
      printf("unable to decrypt\n");
      return 0;
    }

    // print_matrix(output.secret);
    // print_matrix(output.watermark);
    
    for(int i = 0; i < N; i++) {
      for(int j = 0; j < N; j++) {
        set_image_pixel(secret, get_matrix(output.secret, i, j), pixelOut);
        set_image_pixel(watermark, get_matrix(output.watermark, i, j), pixelOut);
        pixelOut++;
      }
    }
  }

  free_matrix_array(sharesArray);
  close_image(share1);
  close_image(share2);
  // close_image(share3);
  // close_image(share4);
  close_image(rw);

  close_image(secret);
  close_image(watermark);

  return 0;
}
