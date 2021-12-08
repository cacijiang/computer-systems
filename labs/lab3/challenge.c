/* File: mycalloc.c
 * --------------
 * Starter file to implement a version of calloc that copies
 * a specified pattern into allocated memory.
 * Read lab3 writeup for more information.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void *musl_calloc(size_t nelems, size_t elemsz) {
    if (nelems && elemsz > SIZE_MAX/nelems) {
        return NULL;
    }
    size_t total = nelems * elemsz;
    void *p = malloc(total);
    if (p != NULL) {
        long *wp;
        size_t nw = (total + sizeof(*wp) - 1)/sizeof(*wp);
        for (wp = p; nw != 0; nw--,wp++)
            *wp = 0;
    }
    return p;
}

int main(int argc, char *argv[]) {
    return 0;
}
