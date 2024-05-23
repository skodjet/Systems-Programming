// --------------------------- MARCH 16 2023 ------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char *argv[]) {

    
    char *filename = argv[1];
    char *msg = argv[2];

    // Create the file if it doesn't exist. The third argument is for the permissions.
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    int x = 10;
    // char *string = (char*) malloc(100*sizeof(char));
    // strcpy(string, "Here is a sentence that we will write!");


    // This will write 10 in the file because of big endion format (copies only the last byte in x, which is 10).
    // int bytes = write(fd, &x, 1);
    // printf("Number written: %d\n", bytes);

    int bytes = write(fd, msg, strlen(msg));
    printf("Number written: %d\n", bytes);
    

    return 0;
}