#include <stdio.h>
#include <pthread.h>

void* thread_function(void* arg) {
	while (1)
	{
		printf("Thread is running...\n");
	}
	pthread_exit(NULL);
	}

	int main() {
	pthread_t thread;
	int result;

	result = pthread_create(&thread, NULL, thread_function, NULL);

	if (result != 0) {
			return 1;
	}

	result = pthread_cancel(thread);

	if (result != 0) 
	{
		printf("Thread cancellation failed");
		return 1;
	}

	pthread_join(thread, NULL);

	printf("Thread canceled successfully.\n");

	return 0;
}
