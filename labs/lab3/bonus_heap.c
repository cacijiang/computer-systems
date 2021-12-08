/* File: heap.c
 * ------------
 * Code passages to read as part of lab.
 * Read lab3 writeup for more information.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

char *remove_first_occurrence(const char *text, const char *pattern) {
    char *found = strstr(text, pattern);

    if (!found) {
        char *result = strdup(text);
        assert(result != NULL);
        return result;
    }

    int nbefore = found - text;
    int nremoved = strlen(pattern);
    char *result = malloc(strlen(text) + 1 - nremoved);
    assert(result != NULL);

    strncpy(result, text, nbefore);
    strcpy(result + nbefore, found + nremoved);
    return result;
}

void *musl_calloc(size_t nelems, size_t elemsz) {
    if (nelems && elemsz > SIZE_MAX / nelems) {
        return NULL;
    }
    size_t total = nelems * elemsz;
    void *p = malloc(total);
    if (p != NULL) {
        long *wp;
        size_t nw = (total + sizeof(*wp) - 1) / sizeof(*wp);
        for (wp = p; nw != 0; nw--, wp++) {
            *wp = 0;
        }
    }
    return p;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        char *without = remove_first_occurrence("centimeter", "time");
        printf("%s\n", without);
        free(without);
    }
    return 0;
}
