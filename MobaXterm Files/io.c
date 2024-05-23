#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[]) {
    // argv is an array of strings
    // argv[0] is a string
    // argv[1] is a string
    // etc.

    // Print each argument in argv
    // for(int i = 0; i < argc; i++) {
    //     printf("%s\n", argv[i]);
    // }

    // Convert argv[1] to a float and multiply it by 2 (note: will crash if argv[1] is not given)
    // float f = atof(argv[1]);
    // printf("2*f = %f\n", 2*f);

    // Get user input while the program is running
    int mychar = 0;
    char buf[1000];
    int i = 0;
    while((mychar = getchar()) != EOF) {
        buf[i++] = mychar;
    }
    buf[i] = '\0';
    printf("String: %s\n", buf);

    return 0;
}
