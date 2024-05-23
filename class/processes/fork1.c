// --------------------- APRIL 4, 2023 ------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int a = 10;

int main(int argc, char *argv[]) {
    int b = 5;
    char *c = malloc(100 * sizeof(char));

    // Address spaces for the parent and child process are identical.
    printf("Before fork: &a = %p, a = %d, &b = %p, b = %d, c = %p\n", &a, a, &b, b, c);
    

    pid_t pid;

    pid = fork();

    if(pid == 0) {
        // Change values in the child program.
        a = 70;
        b = 843;
        // printf("I am the child.\n");
        // printf("Pid = %d\n\n", getpid());
    } else {
        sleep(1);
        // printf("I am the parent.\n");
        // printf("Pid = %d\n\n", getpid());
    }

    // Address spaces for the parent and the child are still the same, but values in child
    // are different (because we changed them in only the child).
    printf(" After fork: &a = %p, a = %d, &b = %p, b = %d, c = %p\n", &a, a, &b, b, c);

    return 5;
}