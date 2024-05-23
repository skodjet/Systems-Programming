//------------------------------------March 16, 2023-------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

void func() {
    static int i = 0;
    i++;
}

int main(int argc, char *argv[]) {
    clock_t start, end;
    double time_used;

    // Let's see how long a library call takes.

    start = clock();

    for(int i = 0; i < 100000; i++) {
        func();
    }

    end = clock();
    time_used = (double)(end - start)/CLOCKS_PER_SEC;

    // Converting to nanoseconds and finding the average time for func.
    time_used = (time_used * 1e+9) / 100000;

    printf("average time for func: %lf nsecs\n", time_used);



    // Now let's see how long a system call takes.

    start = clock();

    for(int i = 0; i < 100000; i++) {
        close(0);
    }

    end = clock();
    time_used = (double)(end - start)/CLOCKS_PER_SEC;

    // Converting to nanoseconds and finding the average time for close.
    time_used = (time_used * 1e+9) / 100000;

    printf("average time for close: %lf nsecs\n", time_used);

    return 0;
}