/* File: leak.c
 * --------------
 * Code that intentionally leaks memory, to observe under Valgrind.
 * Read lab3 writeup for more information.
 */

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void make_leak_1() {
    printf("--- Leaking memory 1: allocating memory and then function finishes\n");
    void *memory = malloc(8);
    // function returns, address to memory lost!
}

char *make_leak_2() {
    printf("--- Leaking memory 2: allocating memory, returning address, but caller doesn't free\n");
    // memory leak does not occur here; rather, may occur if caller doesn't free!
    return strdup("Hello, world!");
}

void make_leak_3() {
    printf("--- Leaking memory 3: allocating memory, but then losing pointer to it\n");
    char *heap_str = strdup("Hello, world!");
  
    // we reassign the pointer, losing the address of the heap memory!
    heap_str = "Stack string";
    printf("%s\n", heap_str);
}

int main(int argc, char *argv[]) {
    int num = argc > 1 ? atoi(argv[1]) : 0;

    if (num == 1) {
        make_leak_1();
    } else if (num == 2) {
        char *return_value = make_leak_2();
        // return_value goes out of scope, and we lose access to allocated memory!
    } else if (num == 3) {
        make_leak_3();
    } else {
        error(1, 0, "required argument is error number 1, 2 or 3");
    }

    return 0;
}
