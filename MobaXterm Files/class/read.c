#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int my_open(char *filename){
    char errorBuf[100];
    int fd = open(filename, O_RDONLY);
    if(fd < 0){ // ERROR!
        sprintf(errorBuf, "Open %s", filename);
        perror(errorBuf);
        exit(1);
    }
    return fd;
}

int main(int argc, char *argv[]) {
    // Close stdin
    // close(0);

    

    //int fd = my_open("fie.txt");
    

    int fd2 = my_open("file.txt");

    //printf("fd = %d\n", fd);
    printf("fd2 = %d\n", fd2);

    char buf[100];
    for(int i = 0; i < 100; i++) {
        buf[i] = 'x';
    }

    // ----------------------- MARCH 28, 2023 ---------------------------------------
    // Set offset to 5 bytes from the start.
    lseek(fd2, 5, SEEK_SET);

    int bytes = read(fd2, &buf, 10);
    buf[bytes] = '\0';
    printf("number of bytes read: %d\n", bytes);
    printf("buf = %s\n", buf);

    // Read the last 10 bytes of the file
    lseek(fd2, -10, SEEK_END);
    bytes = read(fd2, &buf, 10);
    buf[bytes] = '\0';
    printf("number of bytes read: %d\n", bytes);
    printf("buf = %s\n", buf);

    //close(fd);
    close(fd2);

    return 0;
}