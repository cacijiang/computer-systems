/* This file includes test code for the binsert
 * function implemented as part of assign4.
 * You do not need to modify this file.
 * You can run this program by specifying the -i flag
 * to parse as integers, or -s to parse as strings.  This
 * determines how the remaining command-line arguments are
 * processed.  Then, the remaining arguments are inserted
 * in order into an array via binsert.
 */

#include "samples/prototypes.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <error.h>

/* A comparison function for ints that orders numbers in ascending order. */
int cmp_int(const void *p, const void *q) {
    int first = *(const int *)p;
    int second = *(const int *)q;
    return first - second;
}

int cmp_front(const void *p, const void *q) {
    return 1;
}

void binsert_elt(const void *key, void *base, size_t *p_nelem, size_t width) {
    binsert(key, base, p_nelem, width, cmp_front);
}

/* Test building up an array by parsing each string argument as an int
 * in order and inserting it using binsert.  Asserts if each
 * element was inserted/handled correctly, and prints out the
 * built up array at the end.
 */
void test_int(int count, char *args[]) {
    int arr[count + 1];
    size_t nused = 0;

    for (int i = 0; i < count; i++) {
        int val = atoi(args[i]);
        //int *added = binsert(&val, arr, &nused, sizeof(arr[0]), cmp_int);
        binsert_elt(&val, arr, &nused, sizeof(int));
        //assert(val == *added);
    }

    for (int i = 0; i < nused; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}

/* A comparison function for strings that compares strings in alphabetical order
 * in the same way as strcmp.
 */
int cmp_str(const void *p, const void *q) {
    const char *first = *(const char **)p;
    const char *second = *(const char **)q;
    return strcmp(first, second);
}

/* Test building up an array with each string argument
 * inserted in order using binsert.  Asserts if each
 * element was inserted/handled correctly, and prints out the
 * built up array at the end.
 */
void test_string(int count, char *args[]) {
    char *arr[count + 1];
    size_t nused = 0;

    for (int i = 0; i < count; i++) {
        char *val = args[i];
        char **added = binsert(&val, arr, &nused, sizeof(arr[0]), cmp_str);
        assert(strcmp(val, *added) == 0);
    }

    for (int i = 0; i < nused; i++) {
        printf(" %s", arr[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    /* Tell getopt to not print out its own error messages by setting this
     * special global variable.
     */
    opterr = 0;

    bool is_string = false;
    bool is_int = false;

    int opt = getopt(argc, argv, "is");
    while (opt != -1) {
        if (opt == 'i') {
            is_int = true;
        } else if (opt == 's') {
            is_string = true;
        } else {
            error(1, 0, "Error: invalid command line argument");
        }

        opt = getopt(argc, argv, "is");
    }

    if (is_string) {
        test_string(argc - optind, argv + optind);
    } else if (is_int) {
        test_int(argc - optind, argv + optind);
    }

    return 0;
}

/* Overide the printed program name for the error function so that the
 * solution matches the student implementation error messages for invalid flags.
 */
char *program_invocation_name = "test_binsert";
