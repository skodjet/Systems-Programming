#include <stdio.h>

int doub(int);

int main() {
    int x = 5;
    int y = doub(x); //YES SIRR BIG DUBS

    printf("Hello world!\n");

    return 0;
}

int doub(int a) {
    return 2 * a;
}