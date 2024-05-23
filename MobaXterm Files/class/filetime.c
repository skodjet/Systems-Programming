// ----------------------- MARCH 28, 2023 ----------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {

    struct timeval current;

    gettimeofday(&current, NULL);

    printf("     seconds: %lu\n", current.tv_sec);
    printf("microseconds: %lu\n", current.tv_usec);

    return 0;
}