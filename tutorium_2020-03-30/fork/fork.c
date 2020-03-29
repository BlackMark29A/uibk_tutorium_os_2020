// Standard library headers
#include <assert.h> // Needed for assert()
#include <stdio.h>  // Needed for printf() and perror()
#include <stdlib.h> // Needed for EXIT_SUCCESS and EXIT_FAILURE

// POSIX headers
#include <sys/types.h> // Needed for pid_t
#include <unistd.h>    // Needed for fork() and getpid()

int main() {
	const pid_t id = fork(); // Make it const, because the pid cannot change.

	if(id < 0) {                // Always handle errors!
		perror("Fork failed");  // Use perror to get more information on what went wrong.
		return EXIT_FAILURE;    // Make sure to not continue when a fatal error occurred.
	} else if(id == 0) {        // Fork returns 0 to identify the child, it's not actually 0.
		assert(id != getpid()); // This should prove it.
		printf("I am the child with id %d!\n", getpid());
	} else { // If fork didn't fail and we're not the child, then we have to be the parent.
		printf("I am the parent with id %d and my child is %d!\n", getpid(), id);
	}

	return EXIT_SUCCESS;
}
