#include <pthread.h>
#include <stdio.h>

void* fun_with_args(void* data) {
    int arg = *((int*)data);
    printf("Received argument: %d\n", arg);
    return NULL;
}

int main() {
    int arg = 42;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, fun_with_args, &arg);
    return 0;
}