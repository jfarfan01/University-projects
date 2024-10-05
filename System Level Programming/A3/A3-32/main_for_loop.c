#include <stdio.h>
#include <stdlib.h>

// int int_array [5] = {[0] = 10, [1] = 10, [2] = 10, [3] = 10, [4] = 10 };
int int_array [5];

int main()
{
  for(int i = 0;i < 5; i++)
  {
    int_array[i] = 10;
  }

  return 0;
}