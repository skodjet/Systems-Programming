#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[]) {
    // Assembler does the work for you to convert a float to an int.
    int x = 4.5;
    // printf("x = %d\n", x);

    // TODO: Good exercise to write some standard C functions yourself.

    // Int to String
    x = 123;
    char buf[4];
    sprintf(buf, "%d", x);
    printf("%s\n", buf);

    // String to Int
    // char num[20] = "38584";
    // int val = atoi(num);
    // printf("%d\n", val);
    // printf("%d\n", val + 5);
    // float pi = atof("3.141592");
    // printf("2*pi = %f\n", 2*pi);

    printf("sizeof(int) = %lu\n", sizeof(int));



    return 0;
}