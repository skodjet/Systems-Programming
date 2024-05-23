// --------------------------------- March 23, 2023 --------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "plz specify a file!!!!!!!!!!!!!!!!!!!!!!!!!! >:(\n");
        exit(1);
    }
    char *filename = argv[1];

    DIR *dir;
    if((dir = opendir(filename)) == NULL) {
        perror(filename);
        exit(1);
    }
    
    struct dirent *file;

    while((file = readdir(dir)) != NULL) {
        printf("%-20s\t", file->d_name);
        struct stat buf;
        stat(file->d_name, &buf);

        if(S_ISDIR(buf.st_mode)) {
            printf("(directory)\n");
        }
        if(S_ISLNK(buf.st_mode)) {
            printf("(link)\n");
        }
        if(S_ISREG(buf.st_mode)) {
            printf("(regular file)\n");
        }
    }

    printf("\n");

    // To prevent memory leaks.
    closedir(dir);

    return 0;
}