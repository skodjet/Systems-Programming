/* THIS CODE WAS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. Tommy Skodje */

//TODO:Verify correct includes before submitting
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

    // Usage error
    if(argc > 2) {
        fprintf(stderr, "Error: Usage: mysh [prompt]\n");

        //TODO: What code should I exit with here?
        exit(-1);

    }

    // Print the prompt
    char prompt[1024];
    int noPrompt = 0;
    if(argc == 1) {
        printf("mysh: ");
        strcpy(prompt, "mysh");
    } else if(argc == 2) {
        if(strcmp(argv[1], "-") != 0) {
            printf("%s: ", argv[1]);
            strncpy(prompt, argv[1], 1024);
        } else {
            noPrompt = 1;
        }
    }

    // Get the user's input
    char userInputBuffer[1024];
    char *userInput = fgets(userInputBuffer, 1024, stdin);

    while(userInput != NULL) {

        //TODO:Test. Remove.
        printf("userinput: %s\n", userInput);


        if(noPrompt == 0) {
            printf("%s: ", prompt);
        }
        userInput = fgets(userInputBuffer, 1024, stdin);
    }

    

    return 0;
}