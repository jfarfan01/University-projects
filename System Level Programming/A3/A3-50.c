#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define ONE_GB (1UL << 30)

int main()
{ 
  sleep(4);
  printf("let's allocate\n");
  volatile char* ptr = malloc(10*1024*1024);
  ptr ="hello\0";
  // sleep(2);
  volatile char* ptr2 = malloc(10*1024*1024);
  ptr2 ="hello\0";
  // sleep(2);
  volatile char* ptr3 = malloc(10*1024*1024);
  ptr3 ="hello\0";
  // sleep(2);
  volatile char* ptr4 = malloc(10*1024*1024);
  ptr4 ="hello\0";
  sleep(2);
  printf("1GB for science\n");
  int num_chunks = 100;
  size_t chunk_size = 10 * 1024 * 1024;
  for (int i = 0; i < num_chunks; i++) 
  {
    char *ptr = malloc(chunk_size);
    for (int j = 0; j < chunk_size; j++) 
    {
      ptr[j] = 'a';
    }
  
  }
  int * bad = NULL;
  *bad = 4;
    
  return 0;
}

// Everything else can be observed in 49
