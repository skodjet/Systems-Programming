#include<stdio.h>
int sum_array(int*, int);
int ptr_sum(int*, int);

#define ARRAY_LENGTH 10

int main(int argc, char  const *argv[]) {

    int arr[ARRAY_LENGTH];
    for(int i = 0; i < ARRAY_LENGTH; i++) {
        arr[i] = i;
    }

    // int sum = sum_array(arr, ARRAY_LENGTH);

    // printf("Sum = %d\n", sum);
    // printf("arr[0] = %d\n", arr[0]);

    int sum = ptr_sum(arr, ARRAY_LENGTH);
    printf("sum = %d\n", sum);

    return 0;
}

int sum_array(int array[], int length) {
    int sum = 0;
    for(int i = 0; i < length; i++) {
        sum += array[i];
    }
    array[0] = 43; // Changes will propagate to original array.
    return sum;
}


int ptr_sum(int *address_of_array, int length) {
    int *ptr = address_of_array; // To maintain the beginning of the array.

    // printf("address_of_array[0] = %d\n", *ptr);
    // printf("address_of_array[1] = %d\n", *(ptr + 1));
    // printf("address_of_array[2] = %d\n", *(ptr + 2));
    // printf("address_of_array[3] = %d\n", *(ptr + 3));

    // printf("ptr = %p\n", ptr);
    // printf("ptr+1 = %p\n", ptr+1);
    // printf("ptr+2 = %p\n", ptr+2);
    // printf("ptr+3 = %p\n", ptr+3);

    int sum = 0;

    for(int i = 0; i < length; i++) {
        sum += *ptr;
        ptr++;
    }

    return sum;

}