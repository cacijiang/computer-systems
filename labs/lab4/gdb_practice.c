/*
 * CS107
 * Lab 4
 *
 * This file contains example code for learning gdb.  
 * See the lab 4 writeup for more information.
 */

#include <stdlib.h>
#include <stdio.h>

void my_function(int *arr, size_t nelems) {
    for (int i = 0; i < nelems; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {

    int number = 0xffffffff;
    int number2 = 0x88888888;
    int *ptr = &number;
    long number3 = 0x1111111111111111;
    printf("number = %d, number2 = %d, number3 = %ld, ptr = %p\n",
           number, number2, number3, ptr);

    int nums[] = {0, 0xffffffff, 12345, 0x11111111, 25};
    int nelems = sizeof(nums) / sizeof(nums[0]);

    my_function(nums, nelems);

    return 0;
}
