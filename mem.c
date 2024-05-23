// February 21
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[]) {

    char *ptr = sbrk(0);
    sbrk(4097);
    char *after = sbrk(0);
    *ptr = 10;

    //printf("ptr = %p\n", ptr);
    //printf("after = %p\n", after);

    int i = 0;
    while(1) {
        *(ptr + i) = 0;
        i++;
    }

    return 0;
}