#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define SIZE 1000000 

int main()
{
  register static char array[SIZE]; 

  return 0;
}