/* CS 107 Final Project
 * Code by Caci Jiang
 *
 * FILE: explicit.c
 * -------------------------
 * 
 * This program implements a heap allocator using explicit free list. Each block is prefixed by a 8-byte
 * header, which indicates the block size and status (either in-use or free). The free blocks' header 
 * structs are connected as a doubly linked list. The program supports <mymalloc>, <myfree>, and <myrealloc>
 * to mimic the implementations of <malloc>, <free> and <realloc> and manage heap memeory. <mymalloc> will
 * search free block list to find the first available one. This program also supports coalescing and 
 * in-place realloc.
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
static const size_t FREE = 0;
static const size_t ALLOC = 1;

/* STRUCT
 * -------------------------
 *  Description:
 *  the "node" in the doubly linked list, which is composed of a header (8 bytes) for a
 *  free block, a pointer (8 bytes) to the previous free block, and a pointer (8 bytes)
 *  to the next free block.
 */

typedef struct free_header {
    size_t block_size;
    struct free_header *prev;
    struct free_header *next;
} free_header;

static free_header *head; // the head of the doubly linked list in the allocator

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
 * This function would round input size up to be a multiple of mult. If the rounded size
 * is non-zero and still less than the minimal block size (16 bytes for a free block to
 * accommodate two pointers), it will be rounded up to 16 bytes.
 */

size_t roundup(size_t sz, size_t mult) {
    sz = (sz + mult - 1) & ~(mult - 1);
    return (sz > 0 && sz < 2 * mult) ? 2 * mult : sz;
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
 * Returns: a pointer to the header node of an available free block.
 *
 * This function would traverse the doubly linked list of header nodes for free blocks 
 * starting from the head and choose the first free block that fits upon the allocation
 * request each time. If no such appropriate free block can be found in the end, the
 * function will return NULL.
 */

free_header *first_fit(size_t needed_size) {
    free_header *curr_header = head;
    // traverse the header nodes of free blocks only
    while(curr_header != NULL) {
        if (curr_header->block_size >= needed_size) {
            return curr_header;
        }
        curr_header = curr_header->next;
    }
    return NULL;
}

/* FUNCTION: remove_header
 * -----------------------
 * Parameters:
 *      to_remove - a pointer to the header node of the free block that will be removed
 * Returns: N/A
 *
 * This function would remove the header node for a free block from the doubly linked list.
 * In the list, its previous and next header nodes will bedirectly connected. If the header
 * node to remove is the head, then the head will be assigned to be its next node.
 */

void remove_header(free_header *to_remove) {
    // update the pointers information of to_remove's neighbors
    if (to_remove->prev != NULL) {
        to_remove->prev->next = to_remove->next;
    }
    if (to_remove->next != NULL) {
        to_remove->next->prev = to_remove->prev;
    }
    // update head node
    if (head == to_remove) {
        head = to_remove->next;
    }
}

/* FUNCTION: replace_inplace
 * -------------------------
 * Parameters:
 *      orig_header - a pointer to header node of a free block that will be replaced
 *      new_header - a pointer to the header node of a free block that will replace the original one
 * Returns: N/A
 *
 * This function would remove the original header node from the linked list while replacing
 * it with a new header node. The new header node will connect with the original one's previous
 * and next nodes in place. If the header node to be replaced is the head of the list, the head
 * will be updated as the new header node.
 */

void replace_inplace(free_header *orig_header, free_header *new_header) {
    free_header *orig_prev = (orig_header == NULL ? NULL : orig_header->prev);
    free_header *orig_next = (orig_header == NULL ? NULL : orig_header->next);
    // connect the new header node with the original node's neighbors
    // if the neighbor(s) is NULL, set the new node's pointers to be NULL to keep the pointers clean
    if (orig_prev != NULL) {
        orig_prev->next = new_header;
        new_header->prev = orig_prev;
    } else{
        new_header->prev = NULL;
    }
    if (orig_next != NULL) {
        orig_next->prev = new_header;
        new_header->next = orig_next;
    } else {
        new_header->next = NULL;
    }
    // update head information
    if (head == orig_header) {
        head = new_header;
    }
}

/* FUNCTION: create_next_free
 * --------------------------
 * Parameters:
 *      curr_header - a pointer to the header for the free block which will be fragmented
 *      needed_size - the block size needed by the the first partial block
 * Returns: N/A
 *
 * This function would fragment a free block into two, with the first partial one accommondating
 * the needed size and the second partial one in place replacing the original free block in the
 * linked list.
 */

void create_next_free(free_header *curr_header, size_t needed_size) {
    // calculate the address of the new header node for the partial free block
    free_header *new_header = (free_header *) ((char *) curr_header + ALIGNMENT + needed_size);
    // in place connect it in the linked list
    replace_inplace(curr_header, new_header);
    new_header->block_size = set_header(curr_header->block_size - needed_size - ALIGNMENT, FREE);
}

/* FUNCTION: coalesce
 * ------------------
 * Parameters:
 *      left_header - a pointer to the left header node
 *      right_header - a pointer to the right header node
 * Returns: N/A
 *
 * This function would coalesce a free block with its immediate right free block neighbor. The
 * left free node usually indicates a newly added free block. So the coalesced result will be
 * added in place to the linked list, which means to replace the original right header node in
 * place
 */

void coalesce(free_header *left_header, free_header *right_header) {
    replace_inplace(right_header, left_header);
    // assign the combined size to the merged block
    left_header->block_size = set_header(left_header->block_size + ALIGNMENT + right_header->block_size, FREE);
}

/* FUNCTION: add_front
 * -------------------
 * Parameters:
 *      to_add - a pointer to the header node to add at the beginning of the linked list
 * Returns: N/A
 *
 * This function would insert a header node belonging to a free block at the beginning of the
 * linked list and update the head as that node.
 */

void add_front(free_header *to_add) {
    to_add->prev = NULL;
    if (head == NULL) {
        to_add->next = NULL;
    } else {
        to_add->next = head;
        head->prev = to_add;
    }
    head = to_add;
}

/* FUNCTION: check_neighbors
 * ------------------------
 * Parameters:
 *      curr_header - a pointer originally to the header node of the immediate right free block
 *                   from the original allocated block
 *      remaining_size - the size we still need to cover requested by the clients
 * Returns: the number of free blocks
 *
 * This function is usually implemented under the context of re-allocation requests where
 * the new requested size is greater than the original block size. So, this function will
 * check how many contiguous neighbors (free blocks) on the right side of the original block
 * we still need in order to cover the clients' request. The remaining_size denotes the new
 * requested (rounded) minus the original block size.
 */

size_t check_neighbors(free_header *curr_header, size_t remaining_size) {
    size_t count = 0; // the number of contiguous right neighbors
    size_t added_size = 0; // total size of those right neighbors, including their headers

    while (added_size < remaining_size) {
        if (!is_free(curr_header)) {
            // if the sum of the contiguous right neighbors are still not enough
            // we cannot re-allocate in place and will not perform any absorption
            return 0;
        }
        added_size += curr_header->block_size + ALIGNMENT;
        curr_header = get_next_header(curr_header);
        count++;
    }
    return count;
}

/* FUNCTION: absorb_neighbors
* --------------------------
* Parameters:
*      curr_header - a pointer originally to the header node of the immediate right free block
*                   from the original allocated block
*      count - the number of neighbors (free blocks) on the right side we need to absorb
*      needed_size - the total needed size upon the re-allocation request, including the 
*                   original block size
*      remaining_size - the bytes requested by the clients we have not accommodated so far,
*                   excluding the original block size
*
* Returns: the updated block size in total we will allocate for the clients
*
* After we have identified that there would be enough right neighbors (free blocks) for us to
* absorb in order to accommodate an "expanding" re-allocation request, this function will
* implement the absorption for those right neighbors. Given the number of such right neighbors,
* we will iterate and absorb one by one in the order of heap address. At each iteration, the
* function removes the header node of the free block from the linked list and update 
* remaining_size. If we come to the last free block to absorb, this function will check whether
* the last free block needs to fragment if there is enough space left at its end to create a 
* new free block not for the client's use at this time.
*/

size_t absorb_neighbors(free_header *curr_header, size_t count, size_t needed_size, size_t remaining_size) {
    while (count-- > 0) {
        if (count == 0) {
            // caculate the space left at the block end after serving the rest of the requested size
            size_t margin = curr_header->block_size + ALIGNMENT - remaining_size;
            if (margin >= sizeof(free_header)) {
                // if we can fragment the last free block
                create_next_free(curr_header, remaining_size - ALIGNMENT);
                break;
            } else {
                // otherwise add padding to the needed size
                needed_size += margin;
            }
        }
        // remove the header node from the linked list since its block will be used by the clients
        remove_header(curr_header);
        remaining_size -= curr_header->block_size+ ALIGNMENT;
        curr_header = get_next_header(curr_header);
        nblocks--;
    }
    return needed_size;
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
 * Additionally, it will create the first header node and set up the doubly linked list.
 */

bool myinit(void *heap_start, size_t heap_size) {
    if (heap_size < 3 * ALIGNMENT) {
        // the heap segment is too small to ammondate a minimal block with its header
        return false;
    }
    segment_start = heap_start;
    segment_size = heap_size;
    head = segment_start;
    // set the head node with block size
    free_header temp_struct = {set_header(heap_size - ALIGNMENT, FREE), NULL, NULL};
    *head = temp_struct;
    nblocks = 1;
    nused = 0;
    return true;
}

/* FUNCTION: mymalloc
 * ------------------
 * Parameters:
 *      requested_size - the memoery size requested by the clients
 * Returns: a pointer to the memery block available for the clients to use
 *
 * This function would traverse the doubly linked list of the header nodes for free blocks 
 * starting from head until finding the first one that can ammondate the requested size. If 
 * found, the pointer to the block will be returned, which will be marked as in-use, or NULL
 * if no appropriate block. If such free block's size is large enough, it will be fragmented
 * into one allocated for the client to use and another still marked as free. In that case,
 * the newly created partial free block will replace the original free block in place in the
 * linked list.
 */

void *mymalloc(size_t requested_size) {
    // make sure the requested block size is a multiple of "ALIGNMENT"
    size_t needed_size = roundup(requested_size, ALIGNMENT);
    if (needed_size == 0 || needed_size > MAX_REQUEST_SIZE) {
        return NULL;
    }
    // search the linked list of header nodes for free blocks
    free_header *curr_header = first_fit(needed_size);
    if (curr_header != NULL) {
        // get the size that is left to be free after accommodating the requested size in the
        // current available block
        size_t remaining_size = curr_header->block_size - needed_size;
        if (remaining_size >= sizeof(free_header)) {
            // fragment current block if the size left is sufficient to create a new free block
            // add that partial free block to the list in place
            create_next_free(curr_header, needed_size);
            curr_header->block_size = set_header(needed_size, ALLOC);
            nblocks++;
        } else {
            // otherwise add padding to the block and wrap it all for the clients' use
            needed_size += remaining_size;
            curr_header->block_size = set_header(needed_size, ALLOC);
            remove_header(curr_header);
        }
        nused++;
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
 * global values if the input block pointer is not NULL. If there is a free block on the
 * immediate right of the original allocated block, the function will coalesce these two blocks
 * and update the header node in place in the linked list. Otherwise, it just creates a header
 * node for the new free block and add it at the beginning of the linked list.
 */

void myfree(void *ptr) {
    if (ptr != NULL) {
        // get the header pointer
        free_header *curr_header = (free_header *) ((char *) ptr - ALIGNMENT);
        // get the immediate right neighbor's header pointer
        free_header *next_header = (free_header *) get_next_header(curr_header);
        if (is_free(next_header)) {
            // coalesce and update the new free block's information to the linked list in place
            coalesce(curr_header, next_header);
            nblocks--; //nfree doesn't change
        } else {
            // otherwise add the new free block's header node at the beginning of the list
            curr_header->block_size = set_header(curr_header->block_size, FREE);
            add_front(curr_header);
        }
        nused--;
    }
}

/* FUNCTION: myrealloc
 * -------------------
 * Parameters:
 *      old_ptr - a pointer to the allocated block
 *      new_size - new memory size requested by the clients
 * Returns: a pointer to the re-allocated block
 *
 * Special cases: if the input pointer to the previously allocated block is NULL, this function 
 * is equivalent to <mymalloc>. Or the input new size is not valid, this function is equivalent
 * to <myfree>.
 *
 * Case 1: this is a shrinking re-allocation request in terms of requested size. This function
 * will check whether current allocated block needs to fragment by seeing if any space left at
 * its end that would be sufficient to create a minimal free block. If yes, it will create a
 * new header node for that partial free block and add it at the beginning of linked list. Note
 * that for Case 1, there is no need to copy the original data to another memeory location.
 *
 * Case 2: this is an expanding re-allocation request in terms of requested size. This function
 * will check how many contiguous right neighbors (free blocks) we can absorb in order to
 * re-allocate in place. If there are enough neighbors to accommodate the expanding request,
 * this function will absorb those right neighbors and change the corresponding block 
 * information. Otherwise, we cannot re-allocate in place, which means we have to allocate 
 * another block, free the original one and copy our data around.
 */

void *myrealloc(void *old_ptr, size_t new_size) {
    if (old_ptr == NULL) {
        // equivalent to <mymalloc>
        return mymalloc(new_size);
    }
    size_t needed_size = roundup(new_size, ALIGNMENT);

    if (needed_size == 0 || needed_size > MAX_REQUEST_SIZE) {
        myfree(old_ptr);
        return NULL;
    }
    free_header *old_header = (free_header *) ((char *) old_ptr - ALIGNMENT);
    size_t old_size = get_block_size(old_header);
    // Case 1: shrinking request
    if (needed_size <= old_size) {
        size_t remaining_size = old_size - needed_size;
        if (remaining_size >= sizeof(free_header)) {
            // we can fragment the current block
            free_header *new_free = (free_header *) ((char *) old_header + ALIGNMENT + needed_size);
            old_header->block_size = set_header(needed_size, ALLOC);
            new_free->block_size = set_header(remaining_size - ALIGNMENT, FREE);
            add_front(new_free);
            nblocks++;
        }
        return old_ptr;
    } 
    // Case 2: expanding request
    size_t remaining_size = needed_size - old_size;
    free_header *next_header = get_next_header(old_header);
    // check how many contiguous right neighbors we can absorb
    size_t neighbor_count = check_neighbors(next_header, remaining_size);
    if (neighbor_count == 0) {
        // cannot re-allocate in-place
        void *new_ptr = mymalloc(new_size);
        if (new_ptr != NULL) {
            memcpy(new_ptr, old_ptr, old_size);
        }
        myfree(old_ptr);
        return new_ptr;
    }
    // can re-allocate in-place and absorb contiguous right neighbors
    needed_size = absorb_neighbors(next_header, neighbor_count, needed_size, remaining_size);
    // update the original block's size
    old_header->block_size = set_header(needed_size, ALLOC);
    return old_ptr;
}

/* FUNCTION: validate_heap
 * -----------------------
 * Parameters:
 *      N/A
 * Returns: true if the current state of the heap allocator is valid, and false otherwise.
 *
 * This function checks for potential errors/inconsistencies in the heap data structures and
 * returns false if there were issues, or true otherwise. The implementation checks the number
 * of in-use blocks and free blocks ana also, the usage of the bytes in the allocator. In
 * addition to the traversal over all the blocks in the segment, this function traverses
 * across the doubly linked list to check its consistency.
 */

bool validate_heap() {
    void *curr_header = segment_start;
    size_t free_count = 0;
    size_t allocated_count = 0;
    size_t free_bytes = 0;
    size_t allocated_bytes = 0;
    size_t nfree = nblocks - nused;
    for (size_t i = 0; i < nblocks; i++) {
        size_t full_block_size = get_block_size(curr_header) + ALIGNMENT;
        if (is_free(curr_header)) {
            free_count++;
            free_bytes += full_block_size;
        } else {
            allocated_count++;
            allocated_bytes += full_block_size;
        }
        curr_header = get_next_header(curr_header);
    }
    if (free_count != nfree) {
        return false;
    }
    if (allocated_bytes + free_bytes != segment_size) {
        return false;
    }

    free_header *curr_free = head;
    while (curr_free != NULL) {
        if (!is_free(curr_free)) {
            return false;
        }
        if (curr_free->prev && curr_free->prev->next != curr_free) {
            return false;
        }
        free_count--;
        curr_free = curr_free->next;
    }
    if (free_count != 0) {
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
 * be called from gdb to view the contents of the heap segment. For the explicit allocator, it
 * will print the information for all the blocks in the heap segment, including the header,
 * block size and status. Additionally, it will traverse the linked list from head and print
 * the header nodes' information.
 */

void dump_heap() {
    printf("Heap segment starts at address %p, ends at %p. %lu blocks in total.\n", 
            segment_start, (char *)segment_start + segment_size,  nblocks);

    // traverse all the blocks in the order of heap address
    void *curr_header = segment_start;
    for (size_t i = 0; i < nblocks; i++) {
        size_t block_size = get_block_size(curr_header);
        size_t status = is_free(curr_header) ? FREE : ALLOC;
        printf("header #%lu: header address = %p, block size = %lx, status = %lu.\n",
                i, curr_header, block_size, status);
        curr_header = get_next_header(curr_header);
    }

    // traverse the linked list of header nodes from head
    free_header *curr_free = head;
    size_t i = 0;
    while (curr_free != NULL) {
        size_t block_size = curr_free->block_size;
        size_t status = is_free(curr_free) ? FREE : ALLOC;
        printf("free_header #%lu: header address = %p, block size = %lx, status = %lu.\n",
                i, curr_free, block_size, status);
        curr_free = curr_free->next;
        i++;
    }
}
