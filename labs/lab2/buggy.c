/* File: buggy.c
 * --------------
 * Intentionally buggy code to observe under Valgrind.
 * Read lab2 writeup for more information.
 */

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void makeError1() {
    printf("--- Making error 1: strlen called on uninitialized pointer\n\n");
    char *uninitializedPtr;
   
    printf("strlen(uninitializedPtr) = %zu\n", strlen(uninitializedPtr));
}

void makeError2() {
    printf("--- Making error 2: strlen called on string without null terminator\n\n");

    char stackArray[10];
    strncpy(stackArray, "Stanford", 3); 
    printf("strlen(non-terminated) = %zu\n", strlen(stackArray)); 
}

int convertArgument(const char *arg) {
    int num = 0;
    if (arg) {
        num = atoi(arg);
    }
    return num;
}

int main(int argc, char *argv[])
{
    int choice = convertArgument(argv[1]);
    if (choice == 1) {
        makeError1(); 
    } else if (choice == 2) {
        makeError2();
    } else {
        error(1, 0, "required argument is number 1 or 2");
    }

    return 0;
}
