// --------------------- APRIL 4, 2023 ------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>



int main(int argc, char *argv[]) {
    pid_t pid;
    char *filename = argv[1];
    char buf[100];

    if(argc < 2) {
        fprintf(stderr, "Please enter a filename.\n");
        exit(-1);
    }
    filename = argv[1];

    int fd = open(filename, O_RDONLY);

    pid = fork();

    if(pid == 0) {
        // Child
        lseek(fd, 10, SEEK_SET);
    } else {
        // Parent
        sleep(1);
        int end = read(fd, &buf, 20);
        buf[end] = '\0';
        printf("%s\n", buf);
    }

    return 5;
}