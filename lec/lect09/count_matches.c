/* CS107 Lecture 9
 * Generics and Function Pointers
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Function: count_matches
 * -----------------------
 * Takes in information about an array of elements and returns the number of elements
 * that "match" a specified criteria.  This criteria is provided via a function parameter,
 * which can take in a pointer to a single array element and return true if it matches
 * or false otherwise.
 */
int count_matches(void *base, int nelems, int elem_size_bytes, bool (*match_fn)(void *)) {
    // TODO
    return 0;
}

bool match_less_than_three(void *ptr) {
    // TODO
    return false;
}

bool match_nonnegative(void *ptr) {
    // TODO
    return false;
}

int main(int argc, char *argv[]) {
    int nums[5] = {-1, 2, -3, 4, 5};
    int nelems = sizeof(nums) / sizeof(nums[0]);

    // Count number of elements less than 3
    int less_than_three = count_matches(nums, nelems, sizeof(nums[0]), match_less_than_three);
    printf("Found %d matches less than three\n", less_than_three);

    // Count number of nonnegative elements
    int nonnegative = count_matches(nums, nelems, sizeof(nums[0]), match_nonnegative);
    printf("Found %d matches that were nonnegative\n", nonnegative);

    return 0;
}
