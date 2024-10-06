#include <pthread.h>
#include <stdio.h>

void* fun(void* data) {
    printf("Hello from the thread\n");
    return NULL;
}

int main() {
    pthread_t thread_id;
    for (int i = 0; i < 15; i++)
        pthread_create(&thread_id, NULL, fun, NULL);
    return 0;
}