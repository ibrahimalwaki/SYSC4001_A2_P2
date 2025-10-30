// Part II – Step 2: Parent process (Process 1)
// - Increments a counter forever
// - Prints "Cycle number: N" each loop, and when the counter is a multiple of 3
// - Uses exec() to launch the separate child executable (Process 2)
// Build: gcc -O2 -Wall -Wextra -o parent parent.c
// Run:   ./parent  (make sure ./child is compiled in the same folder)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static volatile int running = 1;
static int is_multiple_of(int x, int m) { return (m != 0) && (x % m == 0); }

static void on_term(int sig) {
    (void)sig;
    running = 0;
}

int main(void) {
    // Handle Ctrl+C and kill signals nicely (optional).
    signal(SIGINT,  on_term);
    signal(SIGTERM, on_term);

    const int multiple = 3;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Child of fork(): replace image with the separate child executable.
        // Ensure "child" is compiled in the same directory.
        execl("./child", "./child", (char *)NULL);
        // If execl returns, it failed:
        perror("execl");
        _exit(127);
    }

    // Parent (Process 1): incrementing loop with cycle numbers + multiples-of-3 reporting
    unsigned long cycle = 0;
    int counter = 0;

    printf("[P1 parent] PID=%d launched child PID=%d (multiples of %d)\n",
           getpid(), pid, multiple);
    fflush(stdout);

    while (running) {
        if (is_multiple_of(counter, multiple)) {
            printf("Cycle number: %lu – %d is a multiple of %d\n",
                   cycle, counter, multiple);
        } else {
            printf("Cycle number: %lu\n", cycle);
        }

        counter++;
        cycle++;

        // Slow the display: 200 ms
        usleep(200000);
    }

    // On signal/exit path, just exit. (Spec says to use `ps`/`kill` to stop.)
    printf("[P1 parent %d] exiting\n", getpid());
    return 0;
}

// gcc -O2 -Wall -Wextra -o child child.c
// gcc -O2 -Wall -Wextra -o parent parent.c
// ./parent

// __terminated_by
// ps -ef | grep parent
// ps -ef | grep child
//kill -TERM xx xx xx xx xx 