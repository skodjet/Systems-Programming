// --------------------------- APRIL 18, 2023 -------------------------

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<signal.h>

void handle(){
    char buf[1024] = "\bYou can't kill me that easily!\n";
    write(1, buf, strlen(buf));
}




int main(int argc, char *argv) {
    signal(SIGINT, handle);
    signal(SIGQUIT, handle);
    signal(SIGSTOP, handle);
    while(1) {

    }

    return 0;
}