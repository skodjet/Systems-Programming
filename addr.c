#include<stdio.h>
void incr(int*);

int main(int argc, char const *argv[]) {
    int x = 945;

    char *i = NULL;
    char *c = NULL;

    printf("sizeof(i) = %lu\n", sizeof(i));
    printf("sizeof(c) = %lu\n", sizeof(c));


    // printf("x = %d\n", x);
    // printf("&x = %p\n", &x);

    // C is pass by value, so we need to pass the address to change x.
    incr(&x);
    printf("x = %d\n", x);

    // Pointer
    // int *p = &x;
    // printf("p = %p\n", p);
    // printf("&p = %p\n", &p);
    // printf("*p = %d\n", *p);


    return 0;
}


// Increments the input a
void incr(int *a) {
    printf("*a = %d\n", *a);
    a[0]++;
    printf("*a = %d\n", *a);
}