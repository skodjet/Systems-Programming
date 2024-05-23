// --------------------------- March 21, 2023 ----------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    char *filename = argv[1];


    // fopen is a library call, not a system call.
    FILE * fd = fopen(filename, "r");

    char c;

    // Use fread instead of read
    while((fread(&c, 1, 1, fd))) {

        // Use fwrite instead of write
        fwrite(&c, 1, 1, stdout);
    }

    fclose(fd);

    return 0;
}