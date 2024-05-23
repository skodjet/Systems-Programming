#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main() {

    printf("Here is a printed message!\n");
    fprintf(stderr, "Here is a printed error!\n");

    return 0;
}