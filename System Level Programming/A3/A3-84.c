#include <unistd.h>
#include <stdlib.h>

int main()
{
  sleep(3);
  int b = 0;
  char* a = malloc(4ULL * 1024ULL *1024ULL * 1024ULL);
  for(size_t i = 0; i < 4ULL * 1024ULL *1024ULL * 1024ULL;i++)
    a[i] = i;
  sleep(5);
  fork();
  sleep(5);
  return 0;
  
}