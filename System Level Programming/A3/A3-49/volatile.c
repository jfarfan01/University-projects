#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
  volatile char* ptr = malloc(10*1024*1024);
  sleep(2);
  volatile char* ptr2 = malloc(10*1024*1024);
  sleep(2);
  volatile char* ptr3 = malloc(10*1024*1024);
  sleep(2);
  volatile char* ptr4 = malloc(10*1024*1024);

  return 0;
}