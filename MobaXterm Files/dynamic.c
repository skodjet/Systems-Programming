#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    int *a[2]; // An array of pointers of length 2
    a[0] = (int*)malloc(3*sizeof(int));
    a[1] = (int*)malloc(3*sizeof(int));

    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            //TEST
            a[i][j] = 0;

            printf("%4d", a[i][j]);
        }
        printf("\n");
    }


    return 0;
}