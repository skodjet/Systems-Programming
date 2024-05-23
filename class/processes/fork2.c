// --------------------- APRIL 4, 2023 ------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    pid_t pid;

    pid = fork();

    // Print PID and PPID for the child.
    if(pid == 0) {
        sleep(1);
        printf(" PID = %d\n", getpid());
        printf("PPID = %d\n", getppid());
    }

    return 5;
}