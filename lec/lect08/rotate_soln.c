/**
 * CS107 Lecture 8
 * Code by Jerry Cain
 *
 * File: rotate.c
 * --------------
 * Rotate takes an array of any form whatsoever and exchanges some prefix of
 * the array with the rest of the array.
 */

#include <stdio.h>
#include <string.h>

/**
 * Function: rotate
 * ----------------
 * rotate accepts an array defined by its endpoints (front is the base address, and
 * end is the past-the-end address) and exhanges everything to the left of the
 * supplied separator with everything to the right of the supplied separator.
 *
 * Example:
 *   int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *   rotate(array, array + 5, array + 10); // exchanges the first half with the second half
 *   rotate(array, array + 1, array + 10); // moves the first element to the back after sliding everything else over
 *   rotate(array + 4, array + 5, array + 6);  // exchanges the middle two elements 
 */
void rotate(void *front, void *separator, void *end) {
    int width = (char *)end - (char *)front;
    int prefix_width = (char *)separator - (char *)front;
    int suffix_width = width - prefix_width;
    
    char temp[prefix_width];
    memcpy(temp, front, prefix_width);
    memmove(front, separator, suffix_width);
    memcpy((char *)end - prefix_width, temp, prefix_width);
}

// Prints out an integer array containing the specified number of elements.
void print_int_array(int array[], size_t len) {
    if (len == 0) return;
    printf("Array: %d", array[0]);
    for (size_t i = 1; i < len; i++) {
        printf(", %d", array[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print_int_array(array, 10);

    // exchanges the first half with the second half
    rotate(array, array + 5, array + 10);
    print_int_array(array, 10);

    // moves the first element to the back after sliding everything else over
    rotate(array, array + 1, array + 10);
    print_int_array(array, 10);

    // exchanges the middle two elements
    rotate(array + 4, array + 5, array + 6); 
    print_int_array(array, 10);
    
    return 0;
}
