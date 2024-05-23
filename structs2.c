#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


typedef struct charcharint{
    char c1;
    char c2;
    int i;
} * cci;

int main() {
    char *ptr = sbrk(4096);
    cci x = (cci) ptr;
    cci y = x + 32;
    //x->c1 = 5;

    x = malloc(sizeof(struct charcharint));



    return 0;
}