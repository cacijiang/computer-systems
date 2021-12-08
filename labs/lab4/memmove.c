/* File: code.c
 * ------------
 * Code passages to read as part of lab.
 * Read lab4 writeup for more information.
 */
#include <stdio.h>
#include <string.h>


void *musl_memmove(void *dest, const void *src, size_t nbytes) {
    char *dest_copy = dest;
    const char *src_copy = src;

    if (dest_copy == src_copy) {
        return dest_copy;
    }

    if (src_copy + nbytes <= dest_copy || dest_copy + nbytes <= src_copy) {
        return memcpy(dest_copy, src_copy, nbytes);
    }

    if (dest_copy < src_copy) {
        printf("dest < src\n");
        for (int i = 0; i < nbytes; i++) {
            dest_copy[i] = src_copy[i];
        }
    } else {
        printf("dest >= src\n");
        for (int i = nbytes - 1; i >= 0; i--) {
            dest_copy[i] = src_copy[i];
        }
    }

    return dest;
}


int main(int argc, char *argv[]) {
    musl_memmove(NULL, "cs107", 0);
    musl_memmove(NULL, "cs107", -1);
    return 0;
}
