#include<stdio.h>
#include<stdlib.h>

#define LEN 10

typedef struct charcharint {
    char c1;
    char c2;
    int i;
} cci;

typedef struct charintchar {
    char c1;
    int i;
    char c2;
} cic;

int main(int argc, char const *argv[]) {

    cci s1;
    cic s2;

    // s2 takes more memory than s1
    printf("sizeof(s1) = %lu\n", sizeof(s1));
    printf("sizeof(s2) = %lu\n", sizeof(s2));


    cic *array = (cic*) malloc(LEN * sizeof(long double));

    for(int i = 0; i < LEN; i++) {
        printf("a[%d]: %p\n", i, &array[i]);
    }

}