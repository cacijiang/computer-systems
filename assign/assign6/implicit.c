/* CS 107 Final Project
 * Code by Caci Jiang
 *
 * FILE: implicit.c
 * -------------------------
 * 
 * This program implements a heap allocator using implicit free list. Each block is prefixed by a 8-byte
 * header, which indicates the block size and status (either in-use or free). The program supports
 * <mymalloc>, <myfree>, and <myrealloc> to mimic the implementations of <malloc>, <free> and <realloc>
 * and manage heap memeory. <mymalloc> will search block by block to find the first available free block, 
 * and free blocks can be recycled and reused for subsequent memoery requests.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"
#include "debug_break.h"

/* GLOBAL VARIABLES
 * -------------------------
 */

static void *segment_start;
static size_t segment_size;
static size_t nblocks; // the number of total blocks in the segment
static size_t nused; // the number of in-use blocks in the segment
static size_t used_bytes; // bytes allocated for in-use blocks (including their headers)
static const size_t FREE = 0;
static const size_t ALLOC = 1;

/* HELPER FUNCTIONS
 * -------------------------
 */

/* FUNCTION: roundup
 * -----------------
 * Parameters:
 *      sz - the original size
 *      mult - multiple that for the size to round
 * Returns: a rounded up size which would be the multiple of mult.
 *
 * This function would round input size up to be a multiple of mult.
 */

size_t roundup(size_t sz, size_t mult) {
    return (sz + mult - 1) & ~(mult - 1);
}

/* FUNCTION: set_header
 * --------------------
 * Parameters:
 *      block_size - the size of the block following the header
 *      status - the status of the block (either ALLOC or FREE)
 * Returns: the size of the block to be assigned to the header, with the least 3 significant
 * indicating the block status.
 *
 * This function would assign the block information to the header, including the block size
 * and status.
 */

size_t set_header(size_t block_size, size_t status) {
    return ((block_size >> 1) << 1) | status;
}

/* FUNCTION: get_block_size
 * ------------------------
 * Parameters:
 *      header_ptr - a pointer to the start of the header
 * Returns: the size of the block following this header.
 *
 * This function would get the block size given the corresponding header pointer.
 */

size_t get_block_size(void *header_ptr) {
    return (*(size_t *) header_ptr >> 1) << 1;
}

/* FUNCTION: get_next_header
 * -------------------------
 * Parameters:
 *      header_ptr - a pointer to the start of the header
 * Returns: a pointer to the next header.
 *
 * This function would get the next header pointer given the current header pointer.
 */

void *get_next_header(void *header_ptr) {
    // advance the header pointer by adding the header size and block size
    return (char *) header_ptr + ALIGNMENT + get_block_size(header_ptr);
}

/* FUNCTION: is_free
 * -----------------
 * Parameters:
 *      header_ptr - a pointer to the start of the header
 * Returns: true if the memory block is free or false if it is in-use
 *
 * This function would check whether the block is free or not given the header pointer.
 */

bool is_free(void *header_ptr) {
    return (*(size_t *) header_ptr & ALLOC) == FREE;
}

/* FUNCTION: first_fit
 * -------------------
 * Parameters:
 *      needed_size - the rounded up size required by the client to allocate memory
 * Returns: a pointer to the start of the header of an available free block.
 *
 * This function would search the list of the blocks from the beginning and choose the first
 * free block that fits upon the allocation request each time. If no such appropriate free
 * block can be found in the end, the function will return NULL.
 */

void *first_fit(size_t needed_size) {
    void *curr_header = segment_start;
    // traverse all the blocks one by one
    for (size_t i = 0; i < nblocks; i++) {
        // the block should be free and have a capacity not less than the needed size
        if (is_free(curr_header) && get_block_size(curr_header) >= needed_size) {
            return curr_header;
        }
        curr_header = get_next_header(curr_header);
    }
    return NULL;
}

/* FUNCTIONS FOR CLIENTS
 * -------------------------
 */

/* FUNCTION: myinit
 * ----------------
 * Parameters:
 *      heap_start - a pointer to the start of the heap segment
 *      heap_size - total size of the heap segment
 * Returns: true if the allocator's initialization is successful and ready for use, and false
 * if the initialization is unsuccessful.
 *
 * This function would initialize the heap allocator by setting the header for the following
 * one large free block and some global values to keep the information about the allocator.
 */

bool myinit(void *heap_start, size_t heap_size) {
    if (heap_size < 2 * ALIGNMENT) {
        // the heap segment is too small to ammondate a minimal block with its header
        return false;
    }
    segment_start = heap_start;
    segment_size = heap_size;
    // mark the first header which is at the beginning of the segment
    *(size_t *) segment_start = set_header(heap_size - ALIGNMENT, FREE);
    nblocks = 1;
    nused = 0;
    used_bytes = 0;
    return true;
}

/* FUNCTION: mymalloc
 * ------------------
 * Parameters:
 *      requested_size - the memoery size requested by the clients
 * Returns: a pointer to the memery block available for the clients to use
 *
 * This function would traverse all the blocks in the heap segment until finding the first free
 * one that can ammondate the requested size. If found, the pointer to the block will be
 * returned, which will be marked as in-use, or NULL if no appropriate block. If such free
 * block's size is large enough, it will be fragmented into one allocated for the client to use
 * and another still marked as free.
 */

void *mymalloc(size_t requested_size) {
    // make sure the requested block size is a multiple of "ALIGNMENT"
    size_t needed_size = roundup(requested_size, ALIGNMENT);
    if (needed_size == 0 || needed_size > MAX_REQUEST_SIZE) {
        return NULL;
    }
    void *curr_header = first_fit(needed_size);
    if(curr_header != NULL) {
        // get the size that is left to be free after accommodating the requested size in the
        // current available block
        size_t remaining_size = get_block_size(curr_header) - needed_size;
        if (remaining_size >= ALIGNMENT * 2) {
            // fragment current block if the size left is sufficient to create a new free block
            *(size_t *) curr_header = set_header(needed_size, ALLOC);
            void *next_header = (char *) curr_header + ALIGNMENT + needed_size;
            *(size_t *) next_header = set_header(remaining_size - ALIGNMENT, FREE);
            nblocks++;
        } else {
            // otherwise add padding to the block and wrap it all for the clients' use
            needed_size += remaining_size;
            *(size_t *) curr_header = set_header(needed_size, ALLOC);
        }
        nused++;
        used_bytes += needed_size + ALIGNMENT;
        // return the block pointer instead of the header pointer
        return (char *) curr_header + ALIGNMENT;
    }
    return NULL;
}

/* FUNCTION: myfree
 * ----------------
 * Parameters:
 *      ptr - a pointer to the allocated block which needs to be freed
 * Returns: N/A.
 *
 * This function would change the status of the given block to free and update the corresponding
 * global values if the input block pointer is not NULL.
 */

void myfree(void *ptr) {
    if (ptr != NULL) {
        void *header = (char *) ptr - ALIGNMENT; // get the header pointer
        size_t block_size = get_block_size(header);
        *(size_t *) header = set_header(block_size, FREE); // update the status to be free
        nused--;
        used_bytes -= block_size + ALIGNMENT;
    }
}

/* FUNCTION: myrealloc
 * -------------------
 * Parameters:
 *      old_ptr - a pointer to the allocated block
 *      new_size - new memory size requested by the clients
 * Returns: a pointer to the re-allocated block
 *
 * This function would allocate a new block for the clients to use given the new requested size
 * and mark the previous used block as free if the input pointer of the previous allocated block
 * is not NULL. Previous data will also be moved to the new allocated block. If the input block
 * pointer is NULL, the implementation would be equivalent to the <mymalloc>.
 */

void *myrealloc(void *old_ptr, size_t new_size) {
    if (old_ptr == NULL) {
        // equivalent to <mymalloc>
        return mymalloc(new_size);
    }
    // note that new size as 0 is also handled by <mymalloc>
    // as new_ptr will be equal to NULL, the rest part is equivalent to <myfree>
    void *new_ptr = mymalloc(new_size);
    if (new_ptr != NULL) {
        memcpy(new_ptr, old_ptr, new_size);
    }
    myfree(old_ptr);
    return new_ptr;
}

/* FUNCTION: validate_heap
 * -----------------------
 * Parameters:
 *      N/A
 * Returns: true if the current state of the heap allocator is valid, and false otherwise.
 *
 * This function checks for potential errors/inconsistencies in the heap data structures and
 * returns false if there were issues, or true otherwise. The implementation checks the number
 * of in-use blocks and free blocks ana also, the usage of the bytes in the allocator.
 */

bool validate_heap() {
    void *curr_header = segment_start;
    size_t free_count = 0; // the number of free blocks in the segment
    size_t allocated_count = 0; // the number of in-use blocks in the segment
    size_t free_bytes = 0; // bytes occupied by free blocks (including their headers)
    size_t allocated_bytes = 0; // bytes occupied by in-use blocks (including their headers)
    for (size_t i = 0; i < nblocks; i++) {
        size_t full_block_size = get_block_size(curr_header) + ALIGNMENT;
        if(is_free(curr_header)) {
            free_count++;
            free_bytes += full_block_size;
        } else {
            allocated_count++;
            allocated_bytes += full_block_size;
        }
        curr_header = get_next_header(curr_header);
    }
    if (allocated_count != nused) {
        return false;
    }
    if (allocated_bytes != used_bytes) {
        return false;
    }
    if (allocated_bytes + free_bytes != segment_size) {
        return false;
    }

    return true;
}

/* FUNCTION: dump_heap
 * -------------------
 * Parameters:
 *      N/A
 * Returns: N/A
 *
 * This function is not called from anywhere, it just serves as a useful debugging aid. It can
 * be called from gdb to view the contents of the heap segment. For the implicit allocator, it
 * will print the information for all the blocks in the heap segment, including the header,
 * block size and status.
 */

void dump_heap() {
    printf("Heap segment starts at address %p, ends at %p. %lu bytes currently used. %lu blocks in total.\n", 
            segment_start, (char *)segment_start + segment_size, used_bytes, nblocks);

    void *curr_header = segment_start;
    for (size_t i = 0; i < nblocks; i++) {
        size_t block_size = get_block_size(curr_header);
        size_t status = is_free(curr_header) ? FREE : ALLOC;
        printf("header #%lu: header address = %p, block size = %lx, status = %lu.\n",
                i, curr_header, block_size, status);
        curr_header = get_next_header(curr_header);
    }
}
