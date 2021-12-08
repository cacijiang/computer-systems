/*
 * CS107
 * Lecture 8
 *
 * This program implements a generic swap function that
 * works for any variable type.  It also shows how you
 * can call the function incorrectly and what happens in
 * memory if you do so.
 */

#include <stdio.h>
#include <string.h>

typedef struct mystruct {
    int x;
    char *s;
} mystruct;

/* This is a generic swap function that can swap the data pointed
 * to by the two pointers, of the given size in bytes.
 */
void swap(void *data1ptr, void *data2ptr, size_t nbytes) {
    char temp[nbytes];
    memcpy(temp, data1ptr, nbytes);
    memcpy(data1ptr, data2ptr, nbytes);
    memcpy(data2ptr, temp, nbytes);
}

int main(int argc, char *argv[]) {
    // Example 1
    int x = 0xffffffff;
    int y = 0xeeeeeeee;
    printf("BEFORE: x = 0x%x, y = 0x%x\n", x, y);
    swap(&x, &y, sizeof(x));
    // swap(&x, &y, sizeof(short));  // what happens if we do this?
    printf("AFTER: x = 0x%x, y = 0x%x\n", x, y);

    // Example 2
    mystruct struct1 = {2, "Hello"};
    mystruct struct2 = {5, "Goodbye"};
    printf("BEFORE: struct1: x = %d, s = %s\n", struct1.x, struct1.s);
    printf("BEFORE: struct2: x = %d, s = %s\n", struct2.x, struct2.s);
    swap(&struct1, &struct2, sizeof(mystruct));
    // swap(&struct1, &struct2, sizeof(int));  // what happens if we do this?
    printf("AFTER: struct1: x = %d, s = %s\n", struct1.x, struct1.s);
    printf("AFTER: struct2: x = %d, s = %s\n", struct2.x, struct2.s);
    return 0;
}
