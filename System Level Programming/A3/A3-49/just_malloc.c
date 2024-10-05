#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
  char* ptr = malloc(10*1024*1024);
  sleep(2);
  char* ptr2 = malloc(10*1024*1024);
  sleep(2);
  char* ptr3 = malloc(10*1024*1024);
  sleep(2);
  char* ptr4 = malloc(10*1024*1024);

  return 0;
}