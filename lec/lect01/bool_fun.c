/* CS107 Lecture 1
 * Lisa Yan
 * ----
 * This program plays with the bool type.
 */
#include <stdio.h>	      // for printf
#include <stdbool.h>	  // for bool

int main(int argc, char *argv[]) {
    // what if we change this instantiation of x?
    bool x = true;

    printf("x as an int: %d\n", x);

    if (x) {
	      printf("A) Hello, world!\n"); // choice A
    } else {
	      printf("B) Howdy, world!\n"); // choice B
    }
    return 0;
}
