//
//  my_malloc-driver.c
//  Lab1: Malloc
//


#include "my_malloc.h"

//TODO:Remove
#include <stdio.h>
//TODO:Remove
#include <string.h>

//TODO:Remove this entire function.
void printFreeList() {
    printf("\nPrinting free list---------\n");
    FreeListNode node = free_list_begin();
    while(node != NULL) {
        

        if(node->flink != NULL) {
            printf("Address: %p *** size: %d *** flink: %p\n", node, node->size, node->flink);
            node = node->flink;
        } else {
            printf("Address: %p *** size: %d *** flink: NULL\n", node, node->size);
            node = node->flink;
        }
    }
}

int main(int argc, const char * argv[])
{
    printFreeList();
    char *var0 = my_malloc(8184);
    printFreeList();
    char *var1 = my_malloc(10001);
    printFreeList();
    my_free(var0);
    printFreeList();
    my_free(var1);
    printFreeList();
    //int *var3 = (int*)my_malloc(sizeof(int) * 3);
    // my_free(var2);
    
    // strcpy(var1, "howdy!");
    // printf("var1 = %s\n", var1);
    // printFreeList();
    // coalesce_free_list();
    // printFreeList();
    // my_free(var3);
    // my_free(var1);

    //printf("sizeof FreeListNode: %lu\n", sizeof(FreeListNode));

    
    

    //my_free(test);
    // for(int i = 0; i < 9999999; i++) {
    //     my_malloc(99999999);
    // }


    return 0;
}


