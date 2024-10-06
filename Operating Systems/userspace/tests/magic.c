#include "stdio.h"
#include "stdlib.h"

int main()
{
    size_t* address = (size_t*) 0xDEADDEAD;
    *address = 5;
    return 0;
}