#ifndef __PARAM_PARSER_H__
#define __PARAM_PARSER_H__

#define ACTION_ENCRYPT 1
#define ACTION_DECRYPT 2
#define ACTION_ERROR -1

struct params {
  int action;
  char *secretImage;
  char *watermark;
  int k;
  int n;
  char *dir;
};

/* 
 * Parse the parameters
 */
struct params parse_params(int argc, char *argv[]);

/*
 * Free the allocated memory of the returned struct
 */
void free_params(struct params params);


#endif
