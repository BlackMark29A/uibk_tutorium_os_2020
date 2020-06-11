#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define NUM_THREADS 16

int fib(int n) {
	if(n < 2) return n;
	return fib(n - 1) + fib(n - 2);
}

void* threadFib(void* arg) {
	const int threadId = (int)(intptr_t)(arg);

	printf("fib(%d) = %d\n", threadId, fib(threadId));

	return NULL;
}

int main() {
	pthread_t threads[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; ++i) {
		if(pthread_create(&threads[i], NULL, threadFib, (void*)(intptr_t)(i)) != 0) {
			perror("Unable to create thread");
			return EXIT_FAILURE;
		}
	}

	for(int i = 0; i < NUM_THREADS; ++i) {
		if(pthread_join(threads[i], NULL) != 0) {
			perror("Unable to join with thread");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
