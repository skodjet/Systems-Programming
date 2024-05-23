#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

void my_print(const char *string) {
    printf("Echo: %s\n", string);
}

int main(int arg, char *argv[]) {
    int ret;
    char buf[1024];

    while((ret = scanf("%s", buf)) != EOF) {
        my_print(buf);
    }
    if(errno != 0) {
        fprintf(stderr, "Uh oh...\n");
        exit(1);
    }

    return 0;
}