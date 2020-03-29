#include <stdio.h>
#include <stdlib.h>

#include <signal.h> // POSIX signal header
/* Note: The signal header of the standard library has the same name!
 * So it depends on the platform or your compiler settings which will be chosen.
 * With gcc you can use POSIX by setting -std=gnu11 and the standard library with -std=c11.
 */

// The handler does not have to be static, but probably should, as it is not part of an interface.
// The function signature is fixed and must be void (int).
static void handler(int signum) {
	(void)signum; // Ignore unused variable warning.

	// TODO: Handle signal
}

int main() {
	struct sigaction sa; // Struct that stores all relevant information about signal behavior.

	sa.sa_handler = handler; // Specify the function `handler` as the function that should be called
	                         // when this signal is received.
	sigemptyset(&sa.sa_mask); // Set the block mask to be empty, i.e. allow all signals to interrupt
	                          // the handler.
	sa.sa_flags = 0; // No special flags, i.e. default behavior. This also means that the handler
	                 // cannot be called again while it is still running.

	// Install the behavior defined in the `sa` struct for the SIGINT signal.
	if(sigaction(SIGINT, &sa, NULL) < 0) {
		perror("Signal handler installation failed");
		return EXIT_FAILURE;
	}

	// TODO: Keep the program running

	return EXIT_SUCCESS;
}
