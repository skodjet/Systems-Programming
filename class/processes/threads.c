#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>

int i = 0;

void *hello() {
    int j = i++;
    printf("Hello from thread %d\n", j);
    return NULL;
}


int main(int argc, char *argv[]) {

    pthread_t tid;

    pthread_create(&tid, NULL, hello, NULL);
    pthread_create(&tid, NULL, hello, NULL);
    pthread_create(&tid, NULL, hello, NULL);

    // pthread_join(tid, NULL);

    sleep(1);
    printf("Goodbye i = %d\n", i);

    return 0;
}