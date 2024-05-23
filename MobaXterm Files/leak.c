#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int* make_array(int length) {
    int *ptr = (int*) malloc(length*sizeof(int));
    return ptr;
}

int main(int argc, char *argv[]) {
    int *array = make_array(50);
    free(array);

    array[100] = 10; // Invalid write (out of bounds of the array generated)
    //This won't give an error, but you can see it's wrong in valgrind.

    return 0;

}