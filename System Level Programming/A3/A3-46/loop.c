#include <stdio.h>
#include <stdlib.h>

int main() {
  int n = 1000000;
  int* arr = malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++)
   {
        arr[i] = i;
    }
  
  return 0;
}
