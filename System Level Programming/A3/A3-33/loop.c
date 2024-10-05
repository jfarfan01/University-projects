#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define SIZE 1000000 

int main()
{
  static char array[SIZE];

  for(int i = 0; i < SIZE; i++)
  {
    array[i] = 'a';
  } 

  return 0;
}