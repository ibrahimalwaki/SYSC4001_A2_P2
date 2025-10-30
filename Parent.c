

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
    signal(SIGINT,  on_term);
    signal(SIGTERM, on_term);

    const int multiple = 3;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
       
        execl("./child", "./child", (char *)NULL);
        // If execl returns, it failed:
        perror("execl");
        _exit(127);
    }

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

        usleep(200000);
    }

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
