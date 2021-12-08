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

char *join(char *strings[], int strings_count) {
    char *result = strdup(strings[0]);
    assert(result != NULL);

    for (int i = 1; i < strings_count; i++) {
        result = realloc(result, strlen(result) + strlen(strings[i]) + 1);
        assert(result != NULL);
        strcat(result, strings[i]);
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        char *joined = join(argv + 1, argc - 1);
        printf("%s\n", joined);
        free(joined);
    }
    return 0;
}
