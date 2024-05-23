// -------------------------------- APRIL 4, 2023 ------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    sleep(5);
    pid_t pid, ppid;
    // printf("Hello world!\n");
    // printf("Goodbye\n");

    pid = getpid();
    ppid = getppid();

    printf("    My pid: %d\n", pid);
    printf("Parent pid: %d\n", ppid);

    return 0;
}