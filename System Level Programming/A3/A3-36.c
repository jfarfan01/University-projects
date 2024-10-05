#include <stdio.h>
#include <stdlib.h>
char some_var[1024ULL*1024ULL*1024ULL*64ULL] = {0x10};

int main()
{
  return 0;
}

// /usr/bin/ld: final link failed: memory exhausted
// collect2: error: ld returned 1 exit status
// gcc -Wall A3-36.c -o 36  10,76s user 36,96s system 60% cpu 1:19,53 total