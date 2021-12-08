/* File: bump.c
 * ------------
 * A "bump" allocator that allocates memory only by tacking on 
 * at the end of the heap.  Free is a no-op: blocks are never coalesced
 * or reused.  Realloc is implemented using malloc/memcpy/free. Operations
 * are fast, but utilization is very poor. It is also missing
 * attention to robustness.
 *
 * This shows the very simplest of approaches; there are better options!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"
#include "debug_break.h"

static void *segment_start;
static size_t segment_size;
static size_t nused;


/* Function: roundup
 * -----------------
 * This function rounds up the given number to the given multiple, which
 * must be a power of 2, and returns the result.  (you saw this code in lab1!).
 */
size_t roundup(size_t sz, size_t mult) {
    return (sz + mult - 1) & ~(mult - 1);
}

/* Function: myinit
 * ----------------
 * This function initializes our global variables based on the specified
 * segment boundary parameters.
 */
bool myinit(void *start, size_t size) {
    segment_start = start;
    segment_size = size;
    nused = 0;
    return true;
}

/* Function: mymalloc
 * ------------------
 * This function satisfies an allocation request by placing
 * the allocated block at the end of the heap.  No search means
 * it is fast, but no memory recycling means very poor utilization.
 */
void *mymalloc(size_t requestedsz) {
    size_t needed = roundup(requestedsz, ALIGNMENT);
    if (needed + nused > segment_size) {
        return NULL;
    }
    void *ptr = (char *)segment_start + nused;
    nused += needed;
    return ptr;
}

/* Function: myfree
 * ----------------
 * This function does nothing - fast!... but lame :(
 */
void myfree(void *ptr) {}

/* Function: realloc
 * -----------------
 * This function satisfies requests for resizing previously-allocated memory
 * blocks by allocating a new block of the requested size and moving the
 * existing contents to that region.  It's not particularly efficient.
 */
void *myrealloc(void *oldptr, size_t newsz) {
    void *newptr = mymalloc(newsz);
    memcpy(newptr, oldptr, newsz);
    myfree(oldptr);
    return newptr;
}

/* Function: validate_heap
 * -----------------------
 * This function checks for potential errors/inconsistencies in the heap data
 * structures and returns false if there were issues, or true otherwise.
 * This implementation checks if the allocator has used more space than is
 * available.
 */
bool validate_heap() {
    if (nused > segment_size) {
        printf("Oops! Have used more heap than total available?!\n");
        breakpoint();   // call this function to stop in gdb to poke around
        return false;
    }
    return true;
}

/* Function: dump_heap
 * -------------------
 * This function is not called from anywhere, it is just here to
 * demonstrate how such a function might be a useful debugging aid.
 * You can then call the function from gdb to view the contents of the heap segment. 
 * For the bump allocator, the heap contains no block headers or 
 * heap housekeeping to provide structure, so all that can be displayed 
 * is a dump of the raw bytes. For a more structured allocator, you 
 * could implement dump_heap to instead just print the block headers, 
 * which would be less overwhelming to wade through for debugging.
 */
void dump_heap() {
    printf("Heap segment starts at address %p, ends at %p. %lu bytes currently used.", 
        segment_start, (char *)segment_start + segment_size, nused);
    for (int i = 0; i < nused; i++) {
        unsigned char *cur = (unsigned char *)segment_start + i;
        if (i % 32 == 0) {
            printf("\n%p: ", cur);
        }
        printf("%02x ", *cur);
    }
}
