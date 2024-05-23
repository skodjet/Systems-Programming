// ----------------------------- APRIL 13, 2023 ------------------------------------


// Piping child to parent. Use dup2
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
    char buf[4097];
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
        dup2(fd[0], 0);
        close(fd[0]);


        // Read what the child wrote
        // 0 = stdin
        int len = read(0, &buf, 4096);
        buf[len] = '\0';
        printf("read: %s\n", buf);
        return 0;
    }
    // Child
    close(fd[0]); // Doesn't need to read
    dup2(fd[1], 1);
    close(fd[1]);


    execlp("ls", "ls", "-l", NULL);


    printf("fd[0]=%d\nfd[1]=%d\n", fd[0], fd[1]);
    // fd[1] is the write end. fd[0] is the read end

    // Write to the pipe and return to the parent.
    // 1 = stdout
    write(1, s, n);

    // close(fd[0]);
    // close(fd[1]);

    return 0;
}