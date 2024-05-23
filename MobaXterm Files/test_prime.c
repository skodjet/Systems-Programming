#include"hw0.h"
#include<stdlib.h>

int main(int argc, char const *argv[]) {

    int x = atoi(argv[1]);

    // for(int i = 2; i < 30; i++) {
    //     prime(i);
    // }

    prime(x);

    return 0;
}