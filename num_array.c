#include <stdio.h>
#include <stdlib.h>

#define LEN 100

// Creates an array of specified length filled in with
// {1, 2, 3, ...}

int* generate(int length) {
    int *arr = (int*) malloc(length * sizeof(int));

    for(int i = 0; i < length; i++) {
        arr[i] = i + 1;
    }

    // printf("Address of arr: %p\n", arr);

    return arr;
}

int main(int argc, char const *argv) {
    int *ptr;
    ptr = generate(LEN);

    for(int i = 0; i < LEN; i++) {
        printf("%8d", ptr[i]);
    }
    printf("\n");
    free(ptr);

    // printf("Address in ptr: %p\n", ptr);

    return 0;

}