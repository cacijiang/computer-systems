/* CS107
 * Lecture 8
 *
 * This program implements a generic function to swap
 * the first and last elements of an array.  It uses
 * the generic swap function we previously wrote.
 */

#include <stdio.h>
#include <string.h>

void swap(void *data1ptr, void *data2ptr, size_t nbytes) {
    char temp[nbytes];
    memcpy(temp, data1ptr, nbytes);
    memcpy(data1ptr, data2ptr, nbytes);
    memcpy(data2ptr, temp, nbytes);
}

void swap_ends(void *arr, size_t nelems, size_t elem_bytes) {
    swap(arr, (char *)arr + (nelems - 1) * elem_bytes, elem_bytes);
}

int main(int argc, char *argv[]) {
    int nums[] = {5, 2, 3, 4, 1};
    size_t nelems = sizeof(nums) / sizeof(nums[0]);
    printf("BEFORE: nums[0] = %d, nums[4] = %d\n", nums[0], nums[4]);
    swap_ends(nums, nelems, sizeof(nums[0]));
    // swap_ends(nums, nelems, sizeof(short));  // what happens if we do this?
    printf("AFTER: nums[0] = %d, nums[4] = %d\n", nums[0], nums[4]);
    return 0;
}

