// --------------------------- March 21, 2023 --------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    char *filename = argv[1];

    int fd = open(filename, O_RDONLY);

    char c;

    // If there's something to read, read will report 1. If it reaches the end, it will report 0.
    while((read(fd, &c, 1))) {

        // Write to stdout 1 byte at a time.
        write(1, &c, 1);
    }

    close(fd);

    return 0;
}