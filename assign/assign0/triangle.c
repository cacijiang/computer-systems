/* File: triangle.c
 * ----------------
 * This program outputs an ascii representation of the Sierpinski triangle.
 *
 * The pattern of stars is generated using a bitwise AND 
 * on the coordinates. This works due to the relationship
 * between the fractal and Pascal's triangle.
 * https://en.wikipedia.org/wiki/Sierpinski_triangle
 */

#include <stdio.h>
#include <stdlib.h>
#include <error.h>

// Define a C constant for the level number to use when not specified by the user
#define DEFAULT_LEVELS 3


/* FUNCTION: print_triangle
 * ------------------------
 * This function prints out a sierpinksi triangle of *s of the given 
 * number of levels, which is assumed to be non-negative.
 */
void print_triangle(int nlevels) {
    int size = 1 << nlevels;

    for (int row = size - 1; row >= 0; row--) {
        for (int col = 0; col < row; col++) {
            printf(" ");
        }
        for (int col = 0; col + row < size; col++) {
            printf("%c ", (col & row) ? ' ' : '*');
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int nlevels = DEFAULT_LEVELS;
    if(argc > 1) {
      nlevels = atoi(argv[1]);
      if(nlevels > 8 || nlevels < 0) {
          error(1, 0, "the number of levels must be between 0 and 8.");
      }
    }
    print_triangle(nlevels);
    return 0;
}
