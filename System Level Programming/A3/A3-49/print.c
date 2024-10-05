#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define ONE_GB (1UL << 30)

int main()
{
  char* ptr = malloc(ONE_GB);
  sleep(3);
  printf("%p", ptr);
  return 0;
}
