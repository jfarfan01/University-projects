#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "extern.h"

int global_int;
int extern_int;

int main()
{
  int stack_int;
  register int register_int;
  int *shared_int = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  //extern
  int* malloc_int = malloc(sizeof(int));

  printf("Function address:  %p\n", main);
  printf("Global int address:%p\n",&global_int);
  printf("Extern int address:%p\n", &extern_int);
  printf("Malloc address:    %p\n",malloc_int);
  printf("Stack address  :   %p\n",&stack_int);
  // printf(" \"register\"address: %p\n",&register_int); //gives compile 
  
  //https://en.wikipedia.org/wiki/File:Program_memory_layout.pdf


  return 0;
}
