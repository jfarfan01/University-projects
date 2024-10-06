#include <pthread.h>
#include <stdio.h>

void* thread_func(void* arg) {
    int oldstate, oldtype;

    // First set and get cancel state and type
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);
    printf("First old cancel state: %d\n", oldstate);

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    printf("First old cancel type: %d\n", oldtype);

    // Second set and get cancel state and type
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstate);
    printf("Second old cancel state: %d\n", oldstate);

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldtype);
    printf("Second old cancel type: %d\n", oldtype);

    return NULL;
}

int main() {
    pthread_t thread;

    pthread_create(&thread, NULL, &thread_func, NULL);
    pthread_join(thread, NULL);

    return 0;
}
