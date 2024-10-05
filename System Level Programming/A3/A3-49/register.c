#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
  register char* ptr;
  ptr = malloc(10*1024*1024);
  sleep(2);
  register char* ptr2;
  ptr2 = malloc(10*1024*1024);
  sleep(2);
  register char* ptr3;
  ptr3 = malloc(10*1024*1024);
  sleep(2);
  register char* ptr4;
  ptr4 = malloc(10*1024*1024);

  return 0;
}