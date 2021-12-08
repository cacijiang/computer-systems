/* File: segment.c
 * ---------------
 * Handles low-level storage underneath the heap allocator. It reserves
 * the large memory segment using the OS-level mmap facility.
 *
 * Written by jzelenski, updated Spring 2018
 */

#include "segment.h"
#include <assert.h>
#include <sys/mman.h>

/* Place segment at fixed address, as default addresses are quite high
 * and easily mistaken for stack addresses.
 */
#define HEAP_START_HINT (void *)0x107000000L

// Static means these variables are only visible within this file
static void *segment_start = NULL;
static size_t segment_size = 0;

void *heap_segment_start() {
    return segment_start;
}

size_t heap_segment_size() {
    return segment_size;
}

void *init_heap_segment(size_t total_size) {
    // Discard any previous segment via munmap
    if (segment_start != NULL) {
        if (munmap(segment_start, total_size) == -1) return NULL;
        segment_start = NULL;
        segment_size = 0;
    }
    
    // Re-initialize by reserving entire segment with mmap
    segment_start = mmap(HEAP_START_HINT, total_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    assert(segment_start != MAP_FAILED);
    segment_size = total_size;
    return segment_start;
}
