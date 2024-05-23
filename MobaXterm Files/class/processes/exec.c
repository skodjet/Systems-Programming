// ------------ APRIL 6, 2023 ---------------------


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Print all of the arguments as pointers, then go one past the arguments
    // for(int i = 0; i <= argc; i++) {
    //     printf("%p\n", argv[i]);
    // }

    // printf("\n");

    // Does the same thing (1 past the last argument is NULL)
    // int j = 0;
    // while(argv[j] != NULL) {
    //     printf("%s\n", argv[j]);
    //     j++;
    // }

    char **my_argv = (char **) malloc((argc+1) * sizeof(char *));
    my_argv[0] = "ls";
    for(int i = 1; i < argc; i++) {
        my_argv[i] = argv[i];
    }

    // Runs ls
    execvp("ls", my_argv);
    //exec() never returns (if successful) so if it reaches the next line, it has failed.
    perror("exec");

    // // Doesn't work (because this version of exec doesn't check the path)
    // execv("ls", my_argv);
    // perror("exec");






    return 0;

}