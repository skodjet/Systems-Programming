//DANGER: YOU ARE ENTERING THE UNGA BUNGA CODE ZONE

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[]) {

    //Aw hell naw man, wtf man? who invited this dude? Get yo ass on ba- (VINE BOOM)
    int *ptr = (int*)malloc(4096);
    int i = 1;
    while(ptr != NULL) {
        printf("%d ptr = %p\n", i, ptr);
        ptr = (int*)malloc(4096000000);
        i++;
    }
    return 0;
}