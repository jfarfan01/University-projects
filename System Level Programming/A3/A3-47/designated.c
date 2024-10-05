#include <stdio.h>
#include <stdlib.h>

int main() {
  int n = 1000000;
  int *array = malloc(n);

  array = {[n] = 150};
  
  return 0;
}
