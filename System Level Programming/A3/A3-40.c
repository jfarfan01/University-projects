#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

char global_var[1 * 1024 * 1024 * 1024];

int main()
{
//   int int_array[600];
//   for(int i = 0; i < 600; i++)
//   {
//     int_array[i] = 600 +87;
//   }

  int a = 20;
  printf("%d\n", a + 6);
  printf("PID: %d\n", getpid());        
  sleep(100);
  return 0;
}