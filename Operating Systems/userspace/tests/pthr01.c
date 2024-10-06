#include <pthread.h>
#include <stdio.h>

int sum = 0;
void* fun(void* data) {
    sum = sum + 1;
    printf("sum now %d\n", sum);
    return (void*) 69;
}

int main() {
    pthread_t thread_id;
    for (int i = 0; i < 15; i++)
        pthread_create(&thread_id, NULL, fun, NULL);
    return 0;
}