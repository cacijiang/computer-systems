/* File: mid.c
 * -----------
 * Program to compute the midpoint of two integers in various ways.
 */
#include <limits.h>
#include <stdio.h>


int midpoint_original(int x, int y) {
    return (x + y) / 2;
}

int midpoint_A(int x, int y) {
    return x + ((y - x) / 2);
}

int midpoint_B(int x, int y) {
    return ((unsigned int)x + (unsigned int)y) >> 1;
}

int midpoint_C(int x, int y) {
    return (x & y) + ((x ^ y) >> 1);
}

void try_mid(int x, int y) {
    printf("\nComputing midpoint of %d and %d\n", x, y);
    printf("mid_orig = %d\n", midpoint_original(x, y));
    printf("mid_A    = %d\n", midpoint_A(x, y));
    printf("mid_B    = %d\n", midpoint_B(x, y));
    printf("mid_C    = %d\n", midpoint_C(x, y));
}

int main(int argc, char *argv[]) {
    // this pair is correct for all mid functions
    try_mid(11, 14);

    // mid_orig will overflow on this pair
    try_mid(INT_MAX, INT_MAX - 2);

    // work through the various mid functions to identify their flaws
    // add pairs of your own that exhibit incorrect result
    return 0;
}

