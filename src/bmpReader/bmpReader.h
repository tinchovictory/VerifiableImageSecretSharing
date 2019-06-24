#ifndef __BMP_READER__
#define __BMP_READER__

typedef struct image * image_t;

/*
 * Open image
 */
image_t open_image(const char * filename);

/*
 * Build new 8 bits image
 */
image_t new_8bit_image(const char * filename, unsigned int width, unsigned int height);

/*
 * Close image
 */
void close_image(image_t image);

/*
 * Get image width
 * Return 0 on error
 */
int get_image_width(image_t image);

/*
 * Get image height
 * Return 0 on error
 */
int get_image_height(image_t image);

/*
 * Set pixel
 * Return 0 on error
 * Return 1 on success
 */
int set_image_pixel(image_t image, unsigned char byte, long offset);

/*
 * Get pixel
 * Return 0 on error
 */
unsigned char get_image_pixel(image_t image, long offset);

/*
 * Set reserved byte
 * Return 0 on error
 * Return 1 on success
 */
int set_image_reserved(image_t image, unsigned short value);

/*
 * Get reserved byte
 * Return 0 on error
 */
unsigned short get_image_reserved(image_t image);

/*
 * Get the bytes per pixel
 * Return 0 on error
 */
unsigned short get_image_bytes_pixel(image_t image);

#endif
