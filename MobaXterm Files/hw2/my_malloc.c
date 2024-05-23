/* THIS CODE WAS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. Tommy Skodje */

//
//  my_malloc.c
//  Lab1: Malloc
//

#include "my_malloc.h"
#include<stdio.h>
#include<unistd.h>

MyErrorNo my_errno=MYNOERROR;

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

    FreeListNode currentNode = free_list_begin();

    void * chunkAddress;

    // First chunk allocation.
    if(currentNode == NULL) {
        if(chunkSize < 8192) {
            void *memTest = sbrk(8192);

            // malloc failed.
            if(memTest == (void *)-1) {                
                my_errno = MYENOMEM;
                return NULL;
            }

            chunkAddress = memTest + 8;

        } else {
            void *memTest = sbrk(chunkSize);

            // malloc failed.
            if(memTest == (void *)-1) {                
                my_errno = MYENOMEM;
                return NULL;
            }        

            chunkAddress = memTest + 8;

            // Input the size of the chunk and the magic number.
            int * sizeLocation = (int *)(chunkAddress - (char)8);
            int * magicLocation = (int *)(chunkAddress - (char)4);
            *sizeLocation = chunkSize;
            *magicLocation = 77716884;

            // No need for a free list node, so just return the chunk address.
            return chunkAddress;
        }

        // Align the returned chunk address to 8.
        if((uintptr_t)chunkAddress % 8 != 0) {
           for(int j = 0; j < 7; j++) {
                chunkAddress++;
                if((uintptr_t)chunkAddress % 8 == 0) {
                    break;
                }
           }
        }

        // Input the size of the chunk and the magic number.
        int * sizeLocation = (int *)(chunkAddress - (char)8);
        int * magicLocation = (int *)(chunkAddress - (char)4);

        *sizeLocation = chunkSize;

        // Magic number
        *magicLocation = 77716884;

        
        // Create head of free list
        head = (FreeListNode)(chunkAddress + size + padding);
        head->flink = NULL;
        if(size <= 16) {
            head->size = 8176;
        } else if(size < 8184) {
            head->size = 8192 - chunkSize;
        }      

        // Return the allocated bytes of memory
        return chunkAddress;

    // Not first node allocation.
    } else {
        int foundChunk = 0;
        FreeListNode newNode;

        FreeListNode previousNode;
        FreeListNode initialPos = previousNode;

        FreeListNode nextNode;

        // Search for a free list node large enough.
        while(currentNode != NULL) {

            // Found a chunk large enough.
            if(currentNode->size >= (uint32_t)chunkSize) {
                chunkAddress = (void *)currentNode + (char)8;
                foundChunk = 1;
                FreeListNode newNode;

                // Split the chunk. Remainder >= 16.
                if(currentNode->size - chunkSize >= 16) {
                    //Offset the free list node and reduce its size.
                    if(free_list_begin() == currentNode) {

                        newNode = (FreeListNode)((void *)currentNode + (uint32_t)chunkSize);

                        newNode->size = currentNode->size - (uint32_t)chunkSize;
                        head = newNode;
                        newNode->flink = currentNode->flink;
                    } else {
                        newNode = (FreeListNode)((void *)currentNode + (uint32_t)chunkSize);

                        newNode->size = currentNode->size - (uint32_t)chunkSize;
                        newNode->flink = currentNode->flink;
                        previousNode->flink = newNode;
                    }

                // Do not split the chunk. Allocate it all
                } else {
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
            }
        }

        // Allocate new memory. No free chunk of sufficient size was found.  
        if(foundChunk == 0) {
            void *memTest;
            if(size <= 8192) {
                memTest = sbrk(8192);
                chunkAddress = memTest + 8;
            } else {
                memTest = sbrk(chunkSize);
                chunkAddress = memTest + 8;
            }

            // malloc failed
            if(memTest == (void *)-1) {
                my_errno = MYENOMEM;
                return NULL;
            }
        }

        // Align the memory to 8 bytes.
        if((uintptr_t)chunkAddress % 8 != 0) {
           for(int j = 0; j < 7; j++) {
                chunkAddress++;
                if((uintptr_t)chunkAddress % 8 == 0) {
                    break;
                }
           }
        }

        // Input the size of the chunk and the magic number.
        int * sizeLocation = (int *)(chunkAddress - (char)8);
        int * magicLocation = (int *)(chunkAddress - (char)4);

        *sizeLocation = chunkSize;

        // Magic number
        *magicLocation = 77716884;       

        return chunkAddress;
    }
}



// Frees the memory at ptr and places it on the free list.
void my_free(void *ptr){

    // Check the magic number
    if(*((int*)ptr - 1) != 77716884 || ptr == NULL) {
        my_errno = MYBADFREEPTR;
    } else {
        int sizeOfChunk = *((int*)(ptr - 8));
        int inserted = 0;
        FreeListNode newNode;

        newNode = (FreeListNode)(ptr - 8);
        newNode->size = sizeOfChunk;

        FreeListNode currentNode = free_list_begin();

        // Case for if there are no nodes in the free list.
        if(currentNode == NULL) {
            head = newNode;
            newNode->flink = NULL;
            inserted = 1;
        }
        FreeListNode previousNode = NULL;
        int isHead = 1;

        // Insert the new node into the free list.
        while(currentNode != NULL) {
            
            // Found the correct address to insert the new free node into.
            if(currentNode > newNode) {

                if(previousNode != NULL) {
                    previousNode->flink = newNode;
                }

                newNode->flink = currentNode;
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

        // Update the head if necessary.
        if(isHead == 1) {
            head = newNode;
        }
        if(inserted == 0) {

            FreeListNode temp = free_list_begin();

            // Find the correct location to insert the new node.
            while(temp < newNode && temp->flink != NULL) {    
                temp = temp->flink;
            }

                newNode->flink = temp->flink;
                temp->flink = newNode;
        }

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

    FreeListNode nextNode;

    // Search for free list nodes that are adjacent.
    while(currentNode != NULL) {
        nextNode = currentNode->flink;

        FreeListNode add = (FreeListNode)((char *)currentNode + (uint32_t)currentNode->size);

        // Found two nodes that are next to each other.
        if(add == nextNode) {
            FreeListNode newNode = currentNode;
            newNode->size = currentNode->size + nextNode->size;
            newNode->flink = nextNode->flink;
            if(head == currentNode) {
                head = newNode;
            }

        // Increment the linked list.
        } else {
            currentNode = currentNode->flink;
        }
    }
}