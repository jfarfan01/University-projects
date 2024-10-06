#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* print_message(void* arg) 
{
    printf("Hello from thread\n");
    return NULL;
}

int main() 
{
    pthread_t threads[5];

    for (int i = 0; i < 5; i++) 
    {
        pthread_create(&threads[i], NULL, print_message, NULL);
    }

    exit(69);
    return 0;
}
