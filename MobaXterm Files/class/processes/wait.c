#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid, cpid;
    int status;

    pid = fork();
    if(pid) {
        // Parent (non-zero pid)
        cpid = wait(&status);
        if(cpid == -1) {
            perror("wait");
            exit(-1);
        }

        printf("Parent: Child (%d) returned\n", cpid);
        printf("Status = %d\n", status);

    } else {
        // Child (zero pid)
        printf("Child (%d)\n", getpid());
        int i = 10;
        return 5;
    }




    return 0;
}