// --------------------- APRIL 4, 2023 ------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    // fork();
    // fork();

    pid = fork();
    pid = fork();

    // printf("I am here.\n");
    //printf("My pid: %d\n", pid);

    if(pid == 0) {
        printf("I am the child.\n");
        printf("Pid = %d\n\n", getpid());
    } else {
        sleep(1);
        printf("I am the parent.\n");
        printf("Pid = %d\n\n", getpid());
    }

    return 0;
}