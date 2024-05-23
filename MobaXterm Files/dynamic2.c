#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 7
#define COLS 7

int main() {

    // ********************February 9th**************************

    // Building the 2D array in the HEAP
    int **a; // In the stack
    a = (int**) malloc(ROWS * sizeof(int*)); // In the heap
    for(int i = 0; i < ROWS; i++) {
        a[i] = (int*) malloc(COLS * sizeof(int)); // Also in the heap
    }
    
    // Initializes 2D array
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            a[i][j] = i + j + 1;
        }
    }

    // Prints the 2D array
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            printf("%4d", a[i][j]);
        }
        printf("\n");
    }

    free(a[1]);
    a[1] = malloc(10*sizeof(int));

    // Prints the address info about the 2D array
    printf("a = %p\n", a);
    for(int i = 0; i < ROWS; i++) {
        printf("a[%d] = %p\n", i, a[i]);
    }

    a[1] = realloc(a[1], 10*sizeof(int));

    int row = 4, col = 3;
    printf("**a = %d\n", *(*a+col)+row);

    // Freeing the 2D array (in proper order).
    for(int i = 0; i < ROWS; i++) {
        free(a[i]);
    }
    free(a);

    return 0;
}