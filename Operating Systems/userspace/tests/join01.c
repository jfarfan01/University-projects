#include <pthread.h>
#include <stdio.h>


void* fun(void* data) 
{
    printf("New thread was created!\n");
    pthread_exit((void*)69);
    return (void*) 1337;
}

int main()
{
    const int NUM_THREADS = 15;
    pthread_t thread_ids[NUM_THREADS];
    void* value_ptrs[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) 
    {
        pthread_create(&thread_ids[i], NULL, fun, NULL);
        pthread_join(thread_ids[i], &value_ptrs[i]);
        printf("Value returned by thread %d: %p\n", i, value_ptrs[i]);
    }
    return 0;
}
