#include <stdio.h>

int main() {
    // Operators: arithmetic, comparison, logic
    int x = 1;

    // False = 0. Every other number is true 
    if(0) {
        printf("This is true\n");
    } else {
        printf("This is false\n");
    }

    // Literals: 4 types

    // Integer
    int a = 10;
    char c = 65;

    // Floating point number
    float f = 1.0;
    double d = 2.0;

    // Character
    char c1 = 'a';
    printf("c1 = %d\n", c1); //Telling printf to print the character as an integer.
    printf("c1 = %c\n", c1); //Telling printf to print the character as a character.

    // String (not primitive)
    printf("Here is a string literal!\n");

    return 0;
}