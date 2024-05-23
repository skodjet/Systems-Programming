// ------------ APRIL 20, 2023 ----------------

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

typedef int (*function) (int, int);

int add(int x, int y) {
    return x + y;
}

int mult(int x, int y) {
    return x * y;
}


int run_op(function op, int x, int y) {
    return op(4, 7);
}

int (*other(int (*op)(int, int)))(int, int) {
    if (op == mult) {
        return add;
    } else {
        return mult;
    }
}


int main(int argc, char *argv[]) {
    int ret = run_op(add, 4, 7);
    printf("4 + 7 = %d\n", ret);
    ret = run_op(mult, 4, 7);
    printf("4 * 7 = %d\n", ret);


    return 0;
}