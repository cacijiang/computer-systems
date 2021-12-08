/* File: my_optional_program.c
 * ---------------------------
 * This program is compiled to use your custom heap allocator
 * when allocating memory.  Optionally write any program you'd
 * like below that allocates memory, using mymalloc, myrealloc,
 * and myfree instead of malloc, realloc and free, and see your
 * heap allocator in action!  (Note: if your code uses indirect 
 * heap-allocating functions like calloc or strdup, you will have
 * to implement this functionality in other ways to have it use
 * your custom heap allocator). 
 *
 * When you compile using `make`, it will create 3 different
 * compiled versions of this program, one using each type of
 * heap allocator.
 */

#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"
#include "segment.h"

#define HEAP_SIZE 1L << 32

bool initialize_heap_allocator() {
    init_heap_segment(HEAP_SIZE);
    return myinit(heap_segment_start(), heap_segment_size());
}

int main(int argc, char *argv[]) {
    if (!initialize_heap_allocator()) {
        return 1;
    }

    // optionally write your code here!
    
    return 0;
}

