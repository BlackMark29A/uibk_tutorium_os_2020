#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h> // Needed for wait()
#include <unistd.h>

int main() {
	for(int i = 0; i < 5; ++i) {
		const pid_t id = fork();

		if(id < 0) {
			perror("Fork failed");
			return EXIT_FAILURE;
		} else if(id == 0) {
			printf("I am the child with id %d!\n", getpid());
			return EXIT_SUCCESS; // Terminate the child, it must not spawn children of its own.
		} else {
			printf("I am the parent with id %d and my child is %d!\n", getpid(), id);
		}
	}

	// Wait as long as there is something to wait on.
	while(wait(NULL) != -1)
		printf("Successfully waited on some child.\n");

	return EXIT_SUCCESS;
}
