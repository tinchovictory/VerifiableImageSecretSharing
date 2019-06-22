#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <errno.h>
#include <string.h>

#include "bmpReader.h"

#define START_HEADER 0x00
#define FILE_SIZE_HEADER 0x02
#define RESERVED_HEADER 0x06
#define IMAGE_OFFSET_HEADER 0x0A
#define HEADER_SIZE_HEADER 0x0E
#define WIDTH_HEADER 0x12
#define HEIGHT_HEADER 0x16
#define COLOR_PLANES_HEADER 0x1A
#define BITES_PER_PIXEL_HEADER 0x1C
#define IMAGE_SIZE_HEADER 0x22
#define COLOR_PALETTE_SIZE_HEADER 0x2E
#define COLOR_PALETTE_START 0X36

/* Internal image data structure */
struct image {
  uint16_t reserved;
  uint32_t imageOffset;
  int32_t width;
  int32_t height;
  uint16_t bytesPerPixel;
  FILE *imgPtr;
};

/* Helper functions definitions */
static uint16_t read_le_uint16(FILE *ptr, long offset);
static uint32_t read_le_uint32(FILE *ptr, long offset);
static void write_le_uint16(FILE *ptr, long offset, uint16_t bytes);
static void write_le_uint32(FILE *ptr, long offset, uint32_t bytes);
static void read_image_headers(image_t image);
static void write_8bit_image_headers(image_t image);



/*
 * Open image
 */
image_t open_image(const char * filename) {
  image_t image;

  if(filename == NULL) {
    return NULL;
  }

  /* Alloc image data structure */
  image = (image_t) malloc(sizeof(struct image));
  if(image == NULL) {
    return NULL;
  }

  /* Open image */
  image->imgPtr = fopen(filename, "r+");
  if(image->imgPtr == NULL) {
    free(image);
    return NULL;
  }

  /* Read image header */
  read_image_headers(image);

  return image;
}

/*
 * Build new 8 bits image
 */
image_t new_8bit_image(const char * filename, unsigned int width, unsigned int height) {
  image_t image;

  if(filename == NULL) {
    return NULL;
  }

  /* Alloc image data structure */
  image = (image_t) malloc(sizeof(struct image));
  if(image == NULL) {
    return NULL;
  }

  /* Create new file */
  image->imgPtr = fopen(filename, "w+");
  if(image->imgPtr == NULL) {
    free(image);
    return NULL;
  }

  /* Set values to image data structure */
  image->width = width;
  image->height = height;
  image->bytesPerPixel = 8;

  /* Write 8 bit image header */
  write_8bit_image_headers(image);

  return image;
}

/*
 * Close image
 */
void close_image(image_t image) {
  if(image == NULL) {
    return;
  }

  fclose(image->imgPtr);
  free(image);
}

/*
 * Get image width
 * Return 0 on error
 */
int get_image_width(image_t image) {
  if(image == NULL) {
    return 0;
  }
  return image->width;
}

/*
 * Get image height
 * Return 0 on error
 */
int get_image_height(image_t image) {
  if(image == NULL) {
    return 0;
  }
  return abs(image->height);
}


/*
 * Set pixel
 * Return 0 on error
 * Return 1 on success
 */
int set_image_pixel(image_t image, unsigned char byte, long offset) {
  if(image == NULL) {
    return 0;
  }

  fseek(image->imgPtr, image->imageOffset + offset, SEEK_SET);
  fwrite(&byte, 1, 1, image->imgPtr);

  return 1;
}

/*
 * Get pixel
 * Return 0 on error
 */
unsigned char get_image_pixel(image_t image, long offset) {
  uint8_t byte = 0;

  if(image == NULL) {
    return 0;
  }

  fseek(image->imgPtr, image->imageOffset + offset, SEEK_SET);
  fread(&byte, 1, 1, image->imgPtr);

  return byte;
}

/*
 * Set reserved byte
 * Return 0 on error
 * Return 1 on success
 */
int set_image_reserved(image_t image, unsigned short value) {
  if(image == NULL) {
    return 0;
  }

  write_le_uint16(image->imgPtr, RESERVED_HEADER, value);

  image->reserved = value;

  return 1;
}

/*
 * Get reserved byte
 * Return 0 on error
 */
unsigned short get_image_reserved(image_t image) {
  if(image == NULL) {
    return 0;
  }
  return image->reserved;
}

/*
 * Get the bytes per pixel
 * Return 0 on error
 */
unsigned short get_image_bytes_pixel(image_t image) {
  if(image == NULL) {
    return 0;
  }
  return image->bytesPerPixel;
}






/* Helper functions */

static uint16_t read_le_uint16(FILE *ptr, long offset) {
  uint8_t buf[2];
  uint16_t result = 0;

  /* Set FILE to the offset */
  fseek(ptr, offset, SEEK_SET);

  /* Read bytes */
  fread(buf, 1, 2, ptr);

  /* Transform to uint16 */
  for(int i = 1; i >= 0; i--) {
    result = (result << 8) | (uint16_t) buf[i];
  }

  return result;
}

static uint32_t read_le_uint32(FILE *ptr, long offset) {
  uint8_t buf[4];
  uint32_t result = 0;

  /* Set FILE to the offset */
  fseek(ptr, offset, SEEK_SET);

  /* Read bytes */
  fread(buf, 1, 4, ptr);

  /* Transform to uint16 */
  for(int i = 3; i >= 0; i--) {
    result = (result << 8) | (uint16_t) buf[i];
  }

  return result;
}

static void write_le_uint16(FILE *ptr, long offset, uint16_t bytes) {
  uint8_t buf[2];

  /* Transform to little endian */
  for(int i = 0; i < 2; i++) {
    buf[i] = (uint8_t) bytes;
    bytes = bytes >> 8;
  }

  /* Set File to the offset */
  fseek(ptr, offset, SEEK_SET);

  /* Write value */
  fwrite(buf, 1, 2, ptr);
}

static void write_le_uint32(FILE *ptr, long offset, uint32_t bytes) {
  uint8_t buf[2];

  /* Transform to little endian */
  for(int i = 0; i < 4; i++) {
    buf[i] = (uint8_t) bytes;
    bytes = bytes >> 8;
  }

  /* Set File to the offset */
  fseek(ptr, offset, SEEK_SET);

  /* Write value */
  fwrite(buf, 1, 4, ptr);
}

static void read_image_headers(image_t image) {
  image->reserved = read_le_uint16(image->imgPtr, RESERVED_HEADER);
  image->imageOffset = read_le_uint32(image->imgPtr, IMAGE_OFFSET_HEADER);
  image->width = (int32_t) read_le_uint32(image->imgPtr, WIDTH_HEADER);
  image->height = (int32_t) read_le_uint32(image->imgPtr, HEIGHT_HEADER);
  image->bytesPerPixel = read_le_uint16(image->imgPtr, BITES_PER_PIXEL_HEADER);
}

static void write_8bit_image_headers(image_t image) {
  fseek(image->imgPtr, START_HEADER, SEEK_SET);

  /* Write start header */
  fputc(0x42, image->imgPtr);
  fputc(0x4D, image->imgPtr);

  /* Write file size */
  uint32_t fileSize = COLOR_PALETTE_START + 1024 + image->width * image->height;
  write_le_uint32(image->imgPtr, FILE_SIZE_HEADER, fileSize);

  /* Image offset */
  uint32_t imageOffset = COLOR_PALETTE_START + 1024;
  write_le_uint32(image->imgPtr, IMAGE_OFFSET_HEADER, imageOffset);
  image->imageOffset = imageOffset;

  /* Header size */
  write_le_uint32(image->imgPtr, HEADER_SIZE_HEADER, 0x28);

  /* Image width */
  write_le_uint32(image->imgPtr, WIDTH_HEADER, image->width);
  
  /* Image height */
  write_le_uint32(image->imgPtr, HEIGHT_HEADER, image->height);

  /* Number of color planes */
  write_le_uint16(image->imgPtr, COLOR_PLANES_HEADER, 0x01);

  /* Number of bytes per pixel */
  write_le_uint16(image->imgPtr, BITES_PER_PIXEL_HEADER, image->bytesPerPixel);

  /* Image size */
  uint32_t imageSize = image->width * image->height;
  write_le_uint32(image->imgPtr, IMAGE_SIZE_HEADER, imageSize);

  /* Number of colors in the color palette */
  write_le_uint32(image->imgPtr, COLOR_PALETTE_SIZE_HEADER, 256);

  /* Fill color palette */
  fseek(image->imgPtr, COLOR_PALETTE_START, SEEK_SET);
  for(int i = 0; i < 256; i++) {
    fputc(i, image->imgPtr);
    fputc(i, image->imgPtr);
    fputc(i, image->imgPtr);
    fputc(0, image->imgPtr);
  } 
}
