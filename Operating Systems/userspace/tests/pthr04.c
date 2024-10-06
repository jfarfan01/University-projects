#include <pthread.h>
#include <stdio.h>

#define RECURSION_DEPTH 10
#define ARRAY_SIZE 1000

int recursive_function(int depth) 
{
    char local_array[ARRAY_SIZE];

    for(int i = 0; i < ARRAY_SIZE; i++) 
    {
        local_array[i] = (i + depth) % 256;
    }

    if (depth > 0) 
    {
        return local_array[depth % ARRAY_SIZE] + recursive_function(depth - 1);
    }  
        return local_array[0];
}

void* fun_with_args(void* data) 
{
    int arg = *((int*)data);
    printf("Received argument: %d\n", arg);
    int result = recursive_function(RECURSION_DEPTH);
    printf("Recursive function result: %d\n", result);

    return NULL;
}

int main() {
    int arg = 42;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, fun_with_args, &arg);
    return 0;
}