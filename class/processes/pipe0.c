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
    char buf[1024];
    char c;

    int ret = pipe(fd);

    printf("fd[0]=%d\nfd[1]=%d\n", fd[0], fd[1]);

    // //Close the read end. Should send SIGPIPE
    // close(fd[0]);

    // fd[1] is the write end. fd[0] is the read end
    write(fd[1], s, n);
    read(fd[0], &c, 1);

    //Prints the first character of s (pipes are FIFO like a queue)
    printf("c = %c\n", c);

    // Always remember to close your file descriptors! :)
    close(fd[0]);
    close(fd[1]);

    return 0;
}