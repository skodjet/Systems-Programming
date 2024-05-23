/* THIS CODE WAS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. Tommy Skodje */

#include<stdio.h>
#include<stdlib.h>

// Prints out the first n primes separated by commas.
void prime(int n) {
    int primeCounter = 0; // The number of primes that have been printed so far.
    int currentNum = 2;

    while(primeCounter < n) {
        char numIsPrime = 1; // Set to 1 if the current number is prime, 0 if it is not.

        for(int j = 2; j < currentNum; j++) {
            if(currentNum % j == 0) { // The number is not prime.
                numIsPrime = 0;
                break;
            }
        }

            // Print the primes out with the appropriate commas
            if(numIsPrime && primeCounter < (n - 1)) {
                printf("%d, ", currentNum);
                primeCounter++;
            } else if(numIsPrime) {
                printf("%d", currentNum);
                primeCounter++;
            }

            numIsPrime = 1;
            currentNum++;
    }

    printf("\n");

}

// Prints the string in s, and then prints the string in s without vowels on the 
// line beneath.
void rem(const char *s) {
    int length = 0;
    char currentChar = s[0];
    int index = 0;

    // Print the string normally.
    while(currentChar != '\0') {
        printf("%c", currentChar);
        index++;
        currentChar = s[index];
        length++;
    }
    
    printf("\n");

    // Print the string without vowels.
    currentChar = s[0];
    index = 0;
    for(int j = 0; j < length; j++) {
        if(currentChar != 'a' && currentChar != 'e' && currentChar != 'i'
           && currentChar != 'o' && currentChar != 'u' && currentChar != 'A'
           && currentChar != 'E' && currentChar != 'I' && currentChar != 'O'
           && currentChar != 'U') {

                printf("%c", currentChar);
           }
           index++;
           currentChar = s[index];
    }

    printf("\n");
}

// Determines if num is perfect, and prints a statement based on the result.
void perfect(long num) {
    int sumOfFactors = 0;

    for(int j = 1; j < num; j++) {
        if(num % j == 0) {
            sumOfFactors += j;
        }
    }

    //0 is an edge case.
    if(sumOfFactors == num && num != 0) {
        printf("%ld is perfect\n", num);
    } else {
        printf("%ld is not perfect\n", num);
    }

}


// Asks the user to input a number, and then prints the closest prime to that number.
void nearest(void) {
    
    while(1) { // Continue to prompt the user until EOF is given.
        printf("Enter a positive integer: ");
        char currentString[25];
        int index = 0;
        char currentChar = getchar();

        if(currentChar == EOF) {
            printf("\n"); // To make the terminal printout look nicer.
            break;
        }

        while(currentChar != EOF && currentChar != '\n') {
            currentString[index] = currentChar;
            index++;
            currentChar = getchar();
        }
        currentString[index] = '\0';

        // long long type to handle up to 20 digits of input.
        long long currentNum = atoll(currentString);
        char isPrime = 1;

        // Find if the number is prime
        for(int j = 2; j < currentNum; j++) {
            if(currentNum % j == 0) { // currentNum is not prime
                isPrime = 0;
                break;
            }
        }
        
        //Special case. 1 is not prime.
        if(currentNum == 1) {
            printf("The closest prime to 1 is 2\n");
        
        } else if(isPrime) {
            printf("The number %lld is prime.\n", currentNum);

        } else {

            // Find the nearest lower prime
            int lowerPrime = 2;
            int j = 2;
            for(int i = 2; i < currentNum; i++) {
                isPrime = 1;
                for(j = 2; j < i; j++) {
                    if(i % j == 0) {
                        isPrime = 0;
                        break;
                    }
                }
                if(isPrime) {
                    lowerPrime = j;
                }
            }

            // Find the nearest higher prime
            int numToCheck = currentNum + 1;
            int higherPrime = 0;
            while(1) {
                // Check each number larger than the inputted number until a prime is found.
                isPrime = 1;

                for(int j = 2; j < numToCheck; j++) {
                    if(numToCheck % j == 0) {
                        isPrime = 0;
                        break;
                    }
                }

                if(isPrime) {
                    higherPrime = numToCheck;
                    break;
                }
                numToCheck++;
            }

            // Compare the lower and higher primes, and print the nearest one.
            if(currentNum - lowerPrime <= higherPrime - currentNum) {
                printf("The nearest prime to %lld is %d\n", currentNum, lowerPrime);
            } else {
                printf("The nearest prime to %lld is %d\n", currentNum, higherPrime);
            }
        }
    }
}