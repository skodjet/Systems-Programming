#include<stdio.h>
#include<stdlib.h>

int main() {
    int *a = (int*) malloc(10*sizeof(int));
    printf("*a = %d\n", *a);

    free(a);

    a[3] = 10;

    printf("a = %d\n", a);
    printf("*a = %d\n", *a);

    return 0;
}

