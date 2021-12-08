/* CS107
 * Lecture 9
 *
 * This file shows how to implement a generic
 * bubble sort function that uses void * pointers
 * and function pointers.
 */
#include <error.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* This function swaps the values of two integers
 * pointed to by p and q. */
void swap_int(int *p, int *q) {
    int tmp = *p;
    *p = *q;
    *q = tmp;
}

/* This function sorts an array of integers using
 * the bubble sort algorithm, which continually
 * goes through the array swapping pairs of numbers
 * that are out of order until no more pairs need
 * to be swapped. */
void bubble_sort_int(int *arr, int n) {
    while (true) {
        bool swapped = false;
        
        for (int i = 1; i < n; i++) {
            if (arr[i - 1] > arr[i]) {
                swapped = true;
                swap_int(&arr[i - 1], &arr[i]);
            }
        }
        
        if (!swapped) {
            return;
        }
    }
}

// This is a generic function to swap two variables of the same type.
void swap(void *a, void *b, int elem_size_bytes) {
    char temp[elem_size_bytes];
    memcpy(temp, a, elem_size_bytes);
    memcpy(a, b, elem_size_bytes);
    memcpy(b, temp, elem_size_bytes);
}

// This is a standard comparison function for integers.
int int_cmpfn(void *a, void *b) {
    return *(int *)a - *(int *)b;
}

// This is a standard comparison function for strings.
int str_cmpfn(void *a, void *b) {
    char *str1 = *(char **)a;
    char *str2 = *(char **)b;
    return strcmp(str1, str2);
}

/* This is a generic bubble sort function to sort an array of any type.
 * It accepts a comparison function as a parameter to properly compare elements. */
void bubble_sort(void *arr, int n, int elem_size_bytes, int (*cmpfn)(void *a, void *b)) {
    while (true) {
        bool swapped = false;

        for (int i = 1; i < n; i++) {
            void *p_prev_elem = (char *)arr + (i - 1) * elem_size_bytes;
            void *p_curr_elem = (char *)arr + i * elem_size_bytes;
            if (cmpfn(p_prev_elem, p_curr_elem) > 0) {
                swapped = true;
                swap(p_prev_elem, p_curr_elem, elem_size_bytes);
            }
        }

        if (!swapped) {
            return;
        }
    }
}

void test_sort() {
    int nums[] = {4, 2, 12, -5, 56, 14};
    int nums_count = sizeof(nums) / sizeof(nums[0]);
    bubble_sort(nums, nums_count, sizeof(nums[0]), int_cmpfn);
    
    for (int i = 0; i < nums_count; i++) {
        printf("%d  ", nums[i]);
    }
    printf("\n");
}

void test_sort_args(char *args[], int nargs) {
    bubble_sort(args, nargs, sizeof(args[0]), str_cmpfn);

    for (int i = 0; i < nargs; i++) {
        printf("%s ", args[i]);
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    if (argc == 1) {
        test_sort();
    } else {
        test_sort_args(argv + 1, argc - 1);
    }
    
    return 0;
}
