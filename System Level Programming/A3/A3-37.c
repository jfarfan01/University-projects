#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int main()
{
  uint64_t size = (uint64_t) 4 * 1024 * 1024 * 1024;
  printf("%p\n", malloc(size));
  printf("PID: %d\n", getpid());
  sleep(100);
  return 0;
}