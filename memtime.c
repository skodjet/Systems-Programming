#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define PAGESIZE 4096
#define NUM 10000

void slow(void) {
    for(int i = 0; i < NUM; i++) {
        int *ptr = sbrk(PAGESIZE);
        *ptr = 5;
    }

}

// Same as slow, but using malloc instead of sbrk.
void fast(void) {
    for(int i = 0; i < NUM; i++) {
        int *ptr = malloc(PAGESIZE);
        *ptr = 5;
    }

}

int main(int argc, char *argv[]) {
    fast();
    return 0;
}