# Signals

Signals are a way to communicate between processes (IPC, Inter-Process-Communication), even though the amount of transferred information is very limited, they are still useful. Signals stem from Unix, but Windows does have limited signal support through [signal](https://en.cppreference.com/w/c/program/signal) from the C standard library.

If you are writing platform specific programs you should stick to POSIX signals and `sigaction` and not use the signals from the standard library. If you want portability between Linux and Windows you should use the standard library, but will only have a subset of all Unix signals available.

## Installing a signal handler

A signal handler is just a regular user defined function that gets called when a process receives the corresponding signal. A signal handler can be installed using `sigaction` like this:

```c
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
```

_Note:_ Look at `man sigaction` for more information and to see what flags are available.

### Important notes about the handler

There are some very important things to keep in mind when implementing a signal handler. These are:

- Signal handlers cannot send signals themselves
- The state of global variables is undefined
- Modifying global variables is undefined
- All called functions must be async-signal-safe

Pretty much everyone violated one or more of the above. For example by calling `printf` inside the signal handler, which is undefined, because `printf` is not reentrant (and therefore not thread-safe or async-signal-safe). Also reading or writing global variables was a common mistake.

## Correct and safe signal handler

So how is it even possible to write a correct and safe signal handler!? First of all, look at `man signal-safety` for a list of async-signal-safe functions that can be used inside a signal handler. Notice that `printf` is missing, but `write` is on that list, so you can use that instead.

So let's write a correct signal handler that prints the received signal:

```c
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
        const char* errorMessage = "Unable to handle signal!\n";
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
```

_Note:_ If the main function has not handled printing the last signal, the handler drops any new signals with an error message. Printing the error is safe, because it uses `write` instead of `printf`.
Also notice that the signal handler communicates with the main function using `volatile sig_atomic_t` global variables. This is safe, because volatile guarantees that the variables are not cached in registers and therefore all write and read accesses are always visible and the compiler cannot optimize anything. And also `sig_atomic_t` is guaranteed to allow atomic reads and writes.
