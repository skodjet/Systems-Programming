/* THIS CODE WAS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. Tommy Skodje */

//
//  my_malloc.c
//  Lab1: Malloc
//

#include "my_malloc.h"

//TODO:Remove before submitting
#include<stdio.h>

//TODO: Am I allowed to do this?
//#define NULL ((void *)0);
//TODO:And this?
#include<unistd.h>

MyErrorNo my_errno=MYNOERROR;

//TODO: Is this the correct way to do this?
FreeListNode head = NULL;

// Allocates size bytes of memory (just like malloc() would do).
void *my_malloc(uint32_t size){

    // Calculate the chunk size
    int chunkSize;
    int padding = 0;
    if(size <= 8) {
        chunkSize = 16;
    
    } else if(size % 8 == 0) {
        chunkSize = size + 8;
    } else {
        padding = 8 - (size % 8);
        chunkSize = size + padding + 8;
    }

    // Search the free list for a usable chunk.
    FreeListNode currentNode = free_list_begin();

    void * chunkAddress;

    // First chunk allocation.
    if(currentNode == NULL) {
        //TODO: Test. remove.
        printf("head is null\n");
        printf("chunk size: %d\n", chunkSize);

        //TODO:Should I put + 8 after sbrk?

        //TODO:TESTING!-----------------------------------------------------------------------
        if(chunkSize < 8192) {
            void *memTest = sbrk(8192);

            // malloc failed.
            //TODO:Is this the right way to check if you're out of memory?
            if(memTest == (void *)-1) { 
                //TODO:Test. Remove.
                printf("ENOMEM\n");               
                my_errno = MYENOMEM;
                return NULL;
            }

            chunkAddress = memTest + 8;

            //TODO:Test. Remove
            printf("address of chunk: %p\n", chunkAddress);

            

            

        } else {

            void *memTest = sbrk(chunkSize);

            if(memTest == (void *)-1) { 
                //TODO:Test. Remove.
                printf("OUT OF MEMORY!\n");               
                my_errno = MYENOMEM;
                return NULL;
            }      

            chunkAddress = memTest + 8;

             //TODO:Test. Remove
             printf("chunk address: %p\n", chunkAddress);

            // malloc failed.
            //TODO:IS THIS CORRECT?!
              

            /*TODO: Is this correct? There's no need to create a free list node if 
            there's no space for one, right?*/
            // Input the size of the chunk and the magic number.
            int * sizeLocation = (int *)(chunkAddress - (char)8);
            int * magicLocation = (int *)(chunkAddress - (char)4);
            *sizeLocation = chunkSize;
            *magicLocation = 77716884;

            //TODO:Test. Remove.
            printf("sizeLocation: %p\n", sizeLocation);
            printf("magicLocation: %p\n", magicLocation);
            printf("size: %d\n", *sizeLocation);
            printf("magic number: %d\n", *magicLocation);

            return chunkAddress;
        }

        // Align the returned chunk address to 8.
        

        //TODO:Can I use uintptr_t for this?
        if((uintptr_t)chunkAddress % 8 != 0) {
           for(int j = 0; j < 7; j++) {
                chunkAddress++;
                if((uintptr_t)chunkAddress % 8 == 0) {
                    break;
                }
           }
        }

        //TODO:Test. Remove.
        printf("chunkAddress (after aligning): %p\n", chunkAddress);

        // Input the size of the chunk and the magic number.
        int * sizeLocation = (int *)(chunkAddress - (char)8);
        int * magicLocation = (int *)(chunkAddress - (char)4);

        *sizeLocation = chunkSize;

        // Magic number
        *magicLocation = 77716884;

        //TODO:Test. Remove
        printf("sizeLocation: %p\n", sizeLocation);
        printf("chunkSize: %d\n", *sizeLocation);
        printf("magicLocation: %p\n", magicLocation);
        printf("magic number: %d\n", *magicLocation);

        
        // Create head of free list
        //TODO:Test. Remove
        printf("padding: %d\n", padding);
        printf("chunkaddress + size + padding: %p\n", chunkAddress + size + padding);





        //TESTING COMMENTING THIS OUT.
        head = (FreeListNode)(chunkAddress + size + padding);

        head->flink = NULL;

        //TODO:Test. Remove
        printf("made it here!\n");

        if(size <= 16) {
            head->size = 8176;
        } else if(size < 8184) {
            head->size = 8192 - chunkSize;
        }

        //TODO:Test. Remove.
        if(head != NULL) {
            printf("head address: %p\n", head);
            printf("head->flink: %p\n", head->flink);
            printf("head->size: %d\n", head->size);
        }



        

        // Return the allocated byte of memory
        //TODO:Test. Remove.
        printf("Returning address: %p\n", chunkAddress);
        return chunkAddress;

    // Not first node allocation.
    } else {
        //TODO:Test. Remove.
        printf("\nallocating not the first chunk of memory.\n");
        int foundChunk = 0;
        int counter = 0;
        FreeListNode newNode;

        FreeListNode previousNode;
        FreeListNode initialPos = previousNode;

        FreeListNode nextNode;
        while(currentNode != NULL) {

            // Found a chunk large enough.
            if(currentNode->size >= (uint32_t)chunkSize) {
                //TODO:Test. Remove
                printf("Found a chunk big enough at address %p\n", currentNode);
                chunkAddress = (void *)currentNode + (char)8;
                foundChunk = 1;
                FreeListNode newNode;

                // Split the chunk. Remainder >= 16.
                if(currentNode->size - chunkSize >= 16) {
                    //Offset the free list node and reduce its size.
                    if(free_list_begin() == currentNode) {
                        //TODO:Remove
                        printf("current node is the head!\n");

                        newNode = (FreeListNode)((void *)currentNode + (uint32_t)chunkSize);

                        //TODO:Test. Remove.
                        printf("newNode address: %p\n", newNode);

                        newNode->size = currentNode->size - (uint32_t)chunkSize;
                        head = newNode;
                        newNode->flink = currentNode->flink;
                    } else {
                        //TODO:Remove
                        printf("currentnode is NOT the head!\n");
                        newNode = (FreeListNode)((void *)currentNode + (uint32_t)chunkSize);

                        //TODO:Test. Remove.
                        //printf("chunkSize!: %d\n", chunkSize);
                        printf("newNode address: %p\n", newNode);

                        newNode->size = currentNode->size - (uint32_t)chunkSize;
                        newNode->flink = currentNode->flink;
                        previousNode->flink = newNode;
                    }

                // Do not split the chunk. Allocate it all.
                /*TODO:Is this correct? In this case should the node be "deleted" and
                the free list updated?*/
                } else {
                    //TODO:Test. Remove.
                    printf("Not splitting!\n");

                    //TEST
                    chunkSize = currentNode->size;

                    // Reorganize the free list
                        if((previousNode != initialPos) && (previousNode != NULL)) {
                            
                            previousNode->flink = currentNode->flink;
                        }
                    // Assign a new head of the free list if necessary
                        if(free_list_begin() == currentNode) {
                            head = currentNode->flink;
                        }
                }

                break;

            } else {
                previousNode = currentNode;
                currentNode = currentNode->flink;
                counter++;
            }
        }
            
        if(foundChunk == 0) {
            //TODO:test remove
            printf("No free chunks found. Calling sbrk()\n");
            //TODO:Do I need +8 here?
            void *memTest;
            if(size <= 8192) {
                memTest = sbrk(8192);
                chunkAddress = memTest + 8;
            } else {
                memTest = sbrk(chunkSize);
                chunkAddress = memTest + 8;
            }

            if(memTest == (void*)-1) {
                //TODO:Test. remove.
                printf("sbrk failed\n");
                my_errno = MYENOMEM;
                return NULL;
            }
        }

        //TODO:Test. Remove.
        printf("chunkAddress (before aligning): %p\n", chunkAddress);

        //TODO:Can I use uintptr_t for this?
        if((uintptr_t)chunkAddress % 8 != 0) {
           for(int j = 0; j < 7; j++) {
                chunkAddress++;
                if((uintptr_t)chunkAddress % 8 == 0) {
                    break;
                }
           }
        }

        //TODO:Test. Remove.
        printf("chunkAddress (after aligning): %p\n", chunkAddress);

        // Input the size of the chunk and the magic number.
        int * sizeLocation = (int *)(chunkAddress - (char)8);
        int * magicLocation = (int *)(chunkAddress - (char)4);

        *sizeLocation = chunkSize;

        // Magic number
        *magicLocation = 77716884;

        //TODO:Test. Remove
        printf("sizeLocation: %p\n", sizeLocation);
        printf("chunkSize: %d\n", *sizeLocation);
        printf("magicLocation: %p\n", magicLocation);
        printf("magic number: %d\n", *magicLocation);
        printf("returning address: %p\n", chunkAddress);
        

        return chunkAddress;
    }
}



// Frees the memory at ptr and places it on the free list.
void my_free(void *ptr){

    // Check the magic number
    //TODO:remove
    printf("\nFreeing memory at ptr: %p\n", ptr);
    printf("%d\n", *((int*)ptr - 1));

    if(*((int*)ptr - 1) != 77716884 || ptr == NULL) {
        //TODO:Remove
        printf("invalid free\n");
        my_errno = MYBADFREEPTR;
    } else {
        int sizeOfChunk = *((int*)(ptr - 8));

        //TEST-------------------------------------------------------------
        int inserted = 0;

        //TODO:Remove
        printf("size of chunk: %d\n", sizeOfChunk);

        FreeListNode newNode;
        /*TODO:Is -8 correct here? In the assignment it just says to use ptr
        but don't you need to free the size of chunk and magic number to the
        as well?
        */
        newNode = (FreeListNode)(ptr - 8);
        newNode->size = sizeOfChunk;

        // Insert the new node into the free list.
        FreeListNode currentNode = free_list_begin();

        //TODO:Test. Remove.
        printf("currentNode: %p\n", currentNode);
        printf("newNode: %p\n", newNode);

        //TODO:TEST---------------------------------------------------------------
        // Case for if there are no nodes in the free list.
        if(currentNode == NULL) {
            //TODO: Test. REmove.
            printf("made it here\n");
            head = newNode;
            newNode->flink = NULL;
            inserted = 1;
        }

        FreeListNode previousNode = NULL;
        int isHead = 1;
        

        while(currentNode != NULL) {
            
            //TEST
            if(currentNode > newNode) {
                //TODO:Remove
                printf("Inserting newNode into free list\ncurrentNode = %p\n currentNode->flink = %p\n", currentNode, currentNode->flink);
                printf("previousNode: %p\n", previousNode);

                if(previousNode != NULL) {
                    previousNode->flink = newNode;
                }

                newNode->flink = currentNode;

                //TEST-------------------------------------------------------------------
                inserted = 1;
                
                break;

            } else {
                /* If the program searches past at least 1 node, 
                the new node is not the head */
                isHead = 0;

                previousNode = currentNode;
                currentNode = currentNode->flink;
            }
        }

        if(isHead == 1) {
            head = newNode;
        }

        //TEST------------------------------------------------------------------
        // Case for if the head is before the current chunk of memory to be freed.
        if(inserted == 0) {
            
            //TODO:TEST. REMOVE
            printf("made it here!!!\n");
            printf("head: %p\n", free_list_begin());
            printf("head->flink: %p\n", head->flink);
            printf("newNode: %p\n", newNode);

            FreeListNode temp = free_list_begin();

            //TODO:Test. Remove
            // FreeListNode test = temp;
            // while(test != NULL) {
            //     printf("temp: %p\n", test);
            //     test = test->flink;
            // }

            //TEST
            while(temp < newNode && temp->flink != NULL) {
                
                temp = temp->flink;
            }

            
                

                newNode->flink = temp->flink;
                temp->flink = newNode;
        }



        //TODO:Test. Remove.
        printf("newNode address: %p\n", newNode);
        printf("newNode->flink: %p\n", newNode->flink);
        printf("newNode->size: %d\n", newNode->size);


    }

}

// Returns the first node of the free list.
FreeListNode free_list_begin(){
    if(head == NULL) return NULL;

    else return head;
}



// Merges adjacent chunks on the free list together.
void coalesce_free_list(){
    FreeListNode currentNode = free_list_begin();

    //TODO:Test. Remove.
    //printf("currentNode: %p\n", currentNode);

    FreeListNode nextNode;

    while(currentNode != NULL) {
        nextNode = currentNode->flink;

        //TODO:Test. Remove.
        printf("currentNode: %p\n", currentNode);
        printf("nextNode: %p\n", nextNode);
        printf("currentNode->size: %d\n", currentNode->size);
        printf("currentNode + size: %p\n", (FreeListNode)((char *)currentNode + (uint32_t)currentNode->size));

        FreeListNode add = (FreeListNode)((char *)currentNode + (uint32_t)currentNode->size);

        if(add == nextNode) {
            FreeListNode newNode = currentNode;
            newNode->size = currentNode->size + nextNode->size;
            newNode->flink = nextNode->flink;
            if(head == currentNode) {
                head = newNode;
            }

        } else {
            currentNode = currentNode->flink;
        }
    }
}
