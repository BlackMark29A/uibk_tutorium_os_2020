# Fork

Fork is a syscall in Unix operating systems that creates a copy of the current process. These two copies share the same past, but are not connected in the same way that threads of the same process are. The original process that called `fork` is called the _parent_ and the newly cloned process is called the _child_ (Small remark, the plural of child is **children** and not ~~childs~~).

There is no direct way to communicate between parent and child. Global and local variables are _not_ shared between parent and child. This also applies to pointers. The heap is also copied when forking, so pointers are still valid, but the data on the heap is also not shared.

## How to fork

"Just call fork" might be easier said than done. Here's one way of doing it:

```c
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
```

Look at `man fork` for further details.

_Note:_ I've seen some people using `switch-case` to handle the return value of `fork`. I prefer `if` in this case, but I do think that the code looks cleaner with `switch-case`.

## How to wait

Waiting on the children is another very common use-case. There were many people who did this incorrectly in **Task 1** of **Exercise 02**.

Common mistakes were:

- Calculating `fib(40)` before forking and then just using the result. Clever, but not what this task asked for. `fib(40)` was meant to simulate some work for the children.
- Immediately waiting on the child. This defeats the whole purpose of the task. The execution order is not consistent if you spawn multiple children and they do some work (`fib(40)`). If you sequentially create one child and then wait for it to finish the order is obviously deterministic.
  - Saying that the order is not deterministic even though the code immediately waits for the child. This is just incredibly wrong.
- Reading `N`, the number of children to create, from `stdin` and not using the command line parameters as the task requires.

So here's one way of doing it:

```c
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
```

_Note:_ This is a very easy way to wait for all children. Some people stored the `pid`s of all children and then waited on them specifically. While this is not wrong, it is not necessary.
