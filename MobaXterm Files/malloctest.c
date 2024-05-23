#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[]) {

    // malloc() will align to 8 bytes
    char *a = malloc(100);
    char *b = malloc(200);
    char *c = malloc(50);

    // sbrk() does not align to 8 bytes
    // char *a = sbrk(100);
    // char *b = sbrk(200);
    // char *c = sbrk(50);

    printf("a = %p\n", a);
    printf("b = %p\n", b);
    printf("c = %p\n", c);
    printf("------------------\n");

    char *ptr_a = a - 8;
    char *ptr_b = b - 8;
    char *ptr_c = c - 8;

    printf("ptr_a: %p = %d\n", ptr_a, *(int*)ptr_a);
    printf("ptr_b: %p = %d\n", ptr_b, *(int*)ptr_b);
    printf("ptr_c: %p = %d\n", ptr_c, *(int*)ptr_c);

   free(b);

    // Reuses the freed b block.
    // char *d = malloc(200);
    // printf("d = %p\n", d);

    return 0;
}