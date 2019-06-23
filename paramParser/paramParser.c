#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "paramParser.h"

// int main(int argc, char *argv[]) {
//   struct params params = parse_params(argc, argv);
//   if(params.action == ACTION_ERROR) {
//     return 1;
//   }

//   printf("action: %d\n", params.action);
//   printf("secret: %s\n", params.secretImage);
//   printf("watermark: %s\n", params.watermark);
//   printf("k: %d\n", params.k);
//   printf("n: %d\n", params.n);
//   printf("dir: %s\n", params.dir);

//   free_params(params);
// }


static char * copy_str(const char *string);
static void print_usage();


/* 
 * Parse the parameters
 */
struct params parse_params(int argc, char *argv[]) {
  struct params params = {0, NULL, NULL, 0, 0, NULL};
  int opt;

  /* Define long options */
  struct option long_options[] = {
    {"dir", required_argument, NULL, 0},
    {NULL, 0, NULL, 0}
  };

  /* Avoid showing invalid errors */
  opterr = 0;


  while((opt = getopt_long_only(argc, argv, "drs:m:k:n:", long_options, NULL)) != -1) {
    switch(opt) {
      case 'd':
        if(params.action != 0) {
          printf("[ERROR] You can't use -d and -r at the same time\n");
          print_usage();
          params.action = ACTION_ERROR;
          return params;
        }
        params.action = ACTION_ENCRYPT;
        break;
      case 'r':
        if(params.action != 0) {
          printf("[ERROR] You can't use -d and -r at the same time\n");
          print_usage();
          params.action = ACTION_ERROR;
          return params;
        }
        params.action = ACTION_DECRYPT;
        break;
      case 's':
        params.secretImage = copy_str(optarg);
        break;
      case 'm':
        params.watermark = copy_str(optarg);
        break;
      case 'k':
        params.k = atoi(optarg);
        break;
      case 'n':
        params.n = atoi(optarg);
        break;
      case 0:
        params.dir = copy_str(optarg);
        break;
      case '?':
        /* Set action to -1 and return */
        printf("[ERROR]Invalid Paramaters\n");
        print_usage();
        params.action = -1;
        return params;
    }
  }

  /* Check for valid parameters */
  if(params.dir == NULL || params.secretImage == NULL || params.watermark == NULL || params.k == 0 || params.n == 0 || params.action == 0) {
    free_params(params);
    params.action = ACTION_ERROR;
    printf("[ERROR] Missing parameters\n");
    print_usage();
  }

  return params;
}

/*
 * Free the allocated memory of the returned struct
 */
void free_params(struct params params) {
  free(params.secretImage);
  free(params.watermark);
  free(params.dir);
}


static char * copy_str(const char *string) {
  char *str;

  if(string == NULL) {
    return NULL;
  }

  str = calloc(strlen(string) + 1, 1);
  if(str == NULL) {
    return NULL;
  }

  strcpy(str, string);

  return str;
}

static void print_usage() {
  printf("usage [-d or -r] [-s <secretImage>] [-m <watermark>] [-k <value>] [-n <value>] [-dir <directory>]\n");
}