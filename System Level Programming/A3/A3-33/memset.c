#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

#define SIZE 1000000 

int main()
{
  static char array[SIZE]; 
  memset(array, 'a', SIZE);

  return 0;
}