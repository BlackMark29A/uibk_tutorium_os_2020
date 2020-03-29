#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Needed for strlen()

#include <signal.h>
#include <unistd.h> // Needed for write() and pause()

// volatile is needed to make sure the variable is always written to memory.
// sig_atomic_t is a weak atomic type, meaning that it can be read atomically and also written
// atomically, but it cannot be read-modified-written atomically.
// Note that on x86 an int already is a weak atomic type.
volatile sig_atomic_t g_signal = 0;
volatile sig_atomic_t g_flag = false;
volatile sig_atomic_t g_exit = false;

static void handler(int signum) {
	if(g_flag) { // If the flag is still set, drop the signal with an error.
		const char* errorMessage = "Unable to handle signal!";
		const size_t messageLength = strlen(errorMessage);
		// This call is signal-safe, but printf would not be!
		write(STDOUT_FILENO, errorMessage, messageLength);
		return;
	}

	g_signal = signum; // Store which signal was received.
	g_flag = true;     // Notify signal was received.

	if(signum == SIGINT) g_exit = true; // Set exit flag.
}

static bool installHandler(int signum) {
	struct sigaction sa;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if(sigaction(signum, &sa, NULL) < 0) {
		perror("Signal handler installation failed");
		return false;
	}

	return true;
}

int main() {
	// Install the same handler function for a few different signals.
	if(!installHandler(SIGINT) || !installHandler(SIGUSR1) || !installHandler(SIGUSR2))
		return EXIT_FAILURE;

	printf("pid: %d\n", getpid());

	while(!g_exit) {
		pause(); // Block until signal was received and handled.

		if(g_flag) {                                   // Check that there is something to handle.
			printf("Received signal #%d\n", g_signal); // Printf is safe here.
			g_flag = false;                            // Handled signal, reset the flag.
		}
	}

	printf("Exiting\n");

	return EXIT_SUCCESS;
}
