#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

#define SIZE 256 * 1024 * 1024 // 256 MB

char global_array[SIZE];

int main()
{
  char stack_array[SIZE];
  char * malloc_array = malloc(SIZE);
  char *shared_array = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   
  
  for(size_t i = 0; i<SIZE; i++)
  {
    global_array[i] = 9;
    // stack_array[i] = 9;
    // shared_array[i] = 7;
    // malloc_array[i] = 9;
  }

  return 0;
}