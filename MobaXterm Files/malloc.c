#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#define LEN 50

void counter(void) {
    static int i = 0;
    i++;
    printf("i = %d\n");
}

void *my_malloc(unsigned long size) {
    static void *free_heap_ptr = NULL;

    // End heap is the break
    void *end_heap = sbrk(0);

    // Put free heap pointer at the top of the heap.
    if (free_heap_ptr == NULL) {
        free_heap_ptr = end_heap;
    }

    int sz = size + 4;

    // If there's not enough memory, allocate more.
    if(free_heap_ptr + sz > end_heap) {
        sbrk(10000);
    }

    void *ptr = free_heap_ptr;
    free_heap_ptr += sz;

    return ptr + 4;
} 

int main() {

    for(int j = 0; j < 50; j++) {
        counter();
    }

    // char *ptr = sbrk(0);
    // char *c = (char *) malloc(1);
    // char *after = sbrk(0);


    // printf("ptr = %p\n", ptr);
    // printf("after = %p\n", after);
    // printf("difference = %lu\n", after - ptr);

    // int *array = (int*) my_malloc(LEN * sizeof(int));

    // for(int i = 0; i < LEN; i++) {
    //     array[i] = i;
    // }

    // for(int i = 0; i < LEN; i++) {
    //     printf("array[%d] = %d\n", i, array[i]);
    // }





    

    return 0;
}