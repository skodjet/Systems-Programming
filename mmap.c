 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <sys/mman.h>

 int main(int argc, char *argv[]) {
    //char *ptr = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    char *before = sbrk(0);
    char *ptr2 = sbrk(10);

    ptr[0] = 'H';
    printf("ptr[0] = %c\n", ptr[0]);
    munmap(ptr, 4096);
    //brk(before);
    ptr[0] = 'A';

    return 0;
    // int x;
    // char *before = sbrk(0);
    // char *ptr = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    // char *after = sbrk(0);
    // 1 = 0001
    // 2 = 0010
    // 4 = 0100
    // Taking 1 | 2 | 4 = 0111

    // ptr[0] = 'H';
    // printf("ptr[0] = %c\n", ptr[0]);

    // for(int i = 0; i < 4096; i++) {
    //     ptr[i] = 'H';
    // }

    // printf("before = %p\n", before);
    // printf("after = %p\n", after);
    // printf("ptr = %p\n", ptr);
    // printf("&x = %p\n", &x);
    // printf("&x - ptr = %lu\n", (char*)&x - ptr);

    return 0;
 }