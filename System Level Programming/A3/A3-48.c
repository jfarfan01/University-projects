#include <stdlib.h>
#include <stdio.h>

int main() {

    int *ptr;
    ptr = malloc(11LL * 1024LL * 1024LL * 1024LL); // more than my physical memory
    if (ptr == NULL) {
        printf("Physical memory probelm\n");
    }

    int *ptr2;
    ptr2 = malloc(10000000000000); //
    if (ptr2 == NULL) 
    {
      printf("Malloc actually returned 0 (not enough virtual).\n");
    }
    return 0;
}
