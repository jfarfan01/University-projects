#include <pthread.h>
#include <stdio.h>

void* fun(void* data) {

    printf("Hello from the thread\n");
    return NULL;
}

int main() {
    pthread_t thread_id[1];

    pthread_create(&thread_id[0], NULL, fun, NULL);
    pthread_create(&thread_id[1], NULL, fun, NULL);    
    
    pthread_cancel(thread_id[1]);
    return 1;
}