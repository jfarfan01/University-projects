#include <stdio.h>
#include <stdlib.h>


int main() {
    int (*getchar_ptr)(void) = getchar;
    printf("Address of getchar(): %p\n", getchar_ptr);
    return 0;
}