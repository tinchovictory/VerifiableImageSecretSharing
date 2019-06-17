#include <stdio.h>
#include <stdlib.h>

#define MODULUS 251

static int gcd(int a, int b, int *x, int *y) {
  if (a == 0) {
    *x = 0; *y = 1;
    return b;
  }
  int x1, y1;

  int d = gcd(b%a, a, &x1, &y1);
  *x = y1 - (b / a) * x1;
  *y = x1;
  return d;
}

int multiplicative_inverse(int x) {
  int x0, y0, g;
  
  /* Apply modulus to x */
  while(x < 0) {
    x += MODULUS;
  }
  x %= MODULUS;

  g = gcd(abs(x), MODULUS, &x0, &y0);
  if(1 % g) {
    return 0;
  }

  x0 *= 1 / g;
  y0 *= 1 / g;

  if(x < 0){
    x0 = -x0;
  }

  return (x0 + MODULUS) % MODULUS;
}

// int main(void) {
//   int ans;

//   for(int i = 0; i < 251; i++) {
//     ans = multiplicative_inverse(i);
//     if(!ans) {
//       printf("no solution for %d\n", i);
//     } else {
//       printf("solution is %d -> %d\n", i, ans);
//     }
//   }


//       printf("solution is %d -> %d\n", 530, multiplicative_inverse(530));

//   return 0;
// }
