#include <stdio.h>

#include "bmpReader.h"

int main(void) {
  image_t image = open_image("test.bmp");
  if(image == NULL) {
    printf("Unable to open image\n");
    return 1;
  }

  printf("image width: %d\n", get_image_width(image));
  printf("image height: %d\n", get_image_height(image));
  printf("image reserved: %d\n", get_image_reserved(image));

  set_image_reserved(image, 2);
  set_image_pixel(image, 255, 0);
  set_image_pixel(image, 255, 1);
  set_image_pixel(image, 255, 2);

  close_image(image);

  image = new_8bit_image("buildImage.bmp", 100, 200);

  for(int i = 0; i < 200; i++) {
    for(int j = 0; j < 100; j++) {
      set_image_pixel(image, i, i * 100 + j);
    }
  }

  return 0;
}
