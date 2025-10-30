// Part II – Step 2: Child process (Process 2)
// - Decrements a counter forever
// - Prints the cycle number each loop
// - Marks multiples of 3 (including 0)
// Build: gcc -O2 -Wall -Wextra -o child child.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static volatile int running = 1;

static void on_term(int sig) {
    (void)sig;
    running = 0;
}

static int is_multiple_of(int x, int m) { return (m != 0) && (x % m == 0); }

int main(void) {
    signal(SIGINT,  on_term);
    signal(SIGTERM, on_term);

    const int multiple = 3;
    int counter = 0;            // initialize at 0 (as required)
    unsigned long cycle = 0;    // show cycle number every iteration

    printf("[P2 child] PID=%d started (decrementing)\n", getpid());
    fflush(stdout);

    while (running) {
        // Print first (so we see 0 on the first iteration), then decrement.
        if (is_multiple_of(counter, multiple)) {
            printf("Cycle number: %lu – %d is a multiple of %d\n",
                   cycle, counter, multiple);
        } else {
            printf("Cycle number: %lu (counter=%d)\n", cycle, counter);
        }

        // Next state: decrement for the following iteration
        counter--;
        cycle++;

        usleep(200000); // 200 ms
    }

    printf("[P2 child %d] exiting\n", getpid());
    return 0;
}
