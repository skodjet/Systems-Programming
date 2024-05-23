#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

int main(int argc, char *argv[]) {

    char *filename = argv[1];
    struct stat buf;

    // ----------------- March 28, 2023 ------------------------------------------
    lstat(filename, &buf);

    printf("inode number = %lu\n", buf.st_ino);
    printf("size = %lu\n", buf.st_size);

    // %o indicates octal.
    printf("mode = %o\n", buf.st_mode);


    // ------------------------ March 23, 2023 -------------------------------------

    if(S_ISDIR(buf.st_mode)) {
        printf("This is a directory\n");
    }
    if(S_ISLNK(buf.st_mode)) {
        printf("This is a link\n");
    }
    if(S_ISREG(buf.st_mode)) {
        printf("This is a regular file\n");
    }

    // Does the same thing as S_ISREG(buf.st_mode)
    // if(buf.st_mode & 0100000) {
    //     printf("This is a regular file\n");
    // }





    return 0;
}