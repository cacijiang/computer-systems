/* File: buggy.c
 * --------------
 * Intentionally buggy code to observe under Valgrind.
 * Read lab3 writeup for more information.
 */

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


void make_error_1(const char *arg) {
    printf("--- Making error 1: write past end of heap-allocated memory\n\n");
   
    char *ptr = malloc(8);
    assert(ptr != NULL);
    printf("Copying string '%s' (%zu bytes total) to destination heap-allocated 8 bytes.\n",
           arg, strlen(arg) + 1);
    strcpy(ptr, arg);
    free(ptr);
}

void make_error_2() {
    printf("--- Making error 2: access heap memory that has been freed\n\n");

    // strdup does malloc+strcpy
    char *ptr = strdup("Jane Stanford");
    assert(ptr != NULL);
    free(ptr);
    
    // read from freed memory
    printf("Freed memory contains '%s'\n", ptr);
}

void make_error_3() {
    printf("--- Making error 3: double free heap memory\n\n");

    // make heap copy
    char *ptr = strdup("David Starr Jordan");
    assert(ptr != NULL);

    // second pointer to same location
    char *another = ptr;
    
    free(ptr);
    ptr = NULL;

    // this location was already freed!
    free(another);
}

int main(int argc, char *argv[]) {
    int num = argc > 1 ? atoi(argv[1]) : 0;
    char *str =  argc > 2 ? argv[2] : "Stanford";

    if (num == 1) {
        make_error_1(str);
    } else if (num == 2) {
        make_error_2();
    } else if (num == 3) {
        make_error_3();
    } else {
        error(1, 0, "required argument is error number 1, 2 or 3");
    }

    return 0;
}
