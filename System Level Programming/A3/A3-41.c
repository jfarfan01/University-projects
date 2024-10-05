#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>


#define ARRAY_SIZE 1009999
// char arr[ARRAY_SIZE]; 
int main() {
  sleep(5);
    size_t arr[ARRAY_SIZE]; 
    sleep(2);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = i;  
    }
    printf("Finished writing to array\n");
    sleep(2);
    return 0;
}



