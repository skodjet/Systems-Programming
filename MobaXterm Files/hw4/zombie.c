#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/types.h>

int main(int argc, char *argv[]) {
    int pid = fork();

    if(pid < 0) {
        perror("fork");
        exit(1);
    }

    if(pid) {
        // Parent
        while(1);
        return 0;
    }
    // Child
    printf("Help, I've been bitten D:\n");


    return 0;
}