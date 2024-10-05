#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define SIZE 1000000 

int main()
{
  static char array[SIZE] = {[0 ... 999999]= 'a'}; 

  return 0;
}