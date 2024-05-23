// ----------------------------- APRIL 13, 2023 ------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd[2];
    char *s = "Here is text for the pipe!";
    //String length of s
    int n = strlen(s);
    char buf[1025];
    char c;

    int ret = pipe(fd);

    // This part is different from pipe0.c
    int pid = fork();
    if(pid < 0) {
        perror("fork");
        exit(1);
    }

    if(pid) {
        // Parent
        close(fd[1]); // Doesn't need to write
        // Read what the child wrote
        int len = read(fd[0], &buf, 1024);
        buf[len] = '\0';
        printf("read: %s\n", buf);
        return 0;
    }
    // Child
    // sleep(5);

    close(fd[0]); // Doesn't need to read
    printf("fd[0]=%d\nfd[1]=%d\n", fd[0], fd[1]);
    // fd[1] is the write end. fd[0] is the read end

    // Write to the pipe and return to the parent.
    write(fd[1], s, n);

    // close(fd[0]);
    // close(fd[1]);

    return 0;
}