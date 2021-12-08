/* CS 107 assign4
 * Code by Caci Jiang
 *
 * This program has similar operations as standard sort. It will sort the lines given a input
 * file by user-defined sorting order. The default sorting order is lexicographic order. It
 * supports optional command-line flags. "-l" is to sort by line length. "-n" is to sort by
 * string numerical value. "-r" is to sort by reverse order. "-u" is to remove duplicate lines.
 * Whichever flag is last on the command line "wins" as sort order.
 */
#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_LINE_LEN 4096
#define MIN_NLINES 100

typedef int (*cmp_fn_t)(const void *p, const void *q);

/* This is a compare function which accepts two void pointers. They will be casted to strings and
 * compared.
 */
int cmp_pstr(const void *p, const void *q) {
    return strcmp(*(const char **)p, *(const char **)q);
}

/* This is a compare function which accepts two void pointers. They will be casted to strings
 * and compare their lengths.
 */
int cmp_pstr_len(const void *p, const void *q) {
    return strlen(*(const char **)p) - strlen(*(const char **)q);
}

/* This is a compare function which accepts two void pointers. They will be casted to string
 * numerical values by atoi and compared.
 */
int cmp_pstr_numeric(const void *p, const void *q) {
    return atoi(*(const char **)p) - atoi(*(const char **)q);
}

/* This function accepts the pointer to a file stream, a compare function, boolean flag whether
 * to remove duplicate lines, and a boolean flag whether to sort in reverse order. The function
 * will read the lines from the file steam and store them in the heap-allocated and resize-as-
 * you-go array. If the unique flag is invoked, the function will make use of binser() function
 * to insert new unique line or do nothing if current line has already been stored and sort the
 * lines at the same time. If reverse flag is invoked, the function will print the sorted lines
 * in reverse order. Otherwise, it will sort the lines according to normal order specified by
 * the compare functions. Lastly, all the heap-allocated memories will be freed.
 */
void sort_lines(FILE *fp, cmp_fn_t cmp, bool uniq, bool reverse) {
    char stack_line[MAX_LINE_LEN];
    size_t capacity = MIN_NLINES;
    size_t nlines = 0;
    size_t width = sizeof(char *);
    char **heap_lines = malloc(width * capacity);
    assert(heap_lines != NULL);

    while (fgets(stack_line, MAX_LINE_LEN, fp) != NULL) {
        if (!uniq) {
            heap_lines[nlines] = strdup(stack_line);
            assert(heap_lines[nlines] != NULL);
            nlines++;
        } else {
            // Record the original array size to determine whether the new line is inserted later.
            size_t old_nlines = nlines;
            char *key = stack_line;
            char **target = binsert(&key, heap_lines, &nlines, width, cmp);
            if(nlines > old_nlines) {
                *target = strdup(stack_line);
                assert(*target != NULL);
            }
        }
        // Double the array size if the capacity is reached.
        if (nlines == capacity) {
            capacity *= 2;
            heap_lines = realloc(heap_lines, width * capacity);
            assert(heap_lines != NULL);
        }
    }

    if (!uniq) {
        qsort(heap_lines, nlines, width, cmp);
    }
    for (size_t i = 0; i < nlines; i++) {
        size_t idx = i;
        if (reverse) {
            idx = nlines - i - 1;
        }
        printf("%s", heap_lines[idx]);
        free(heap_lines[idx]);
    }
    free(heap_lines);
}

/* This is the main function of the program, which accepts the input file and optional command-
 * line flags. Those flags will determine the compare functions, unique flag and reverse flag.
 * Then it pass the file stream to the sort_lines function to sort the lines and print them.
 */
int main(int argc, char *argv[]) {
    cmp_fn_t cmp = cmp_pstr;
    bool uniq = false;
    bool reverse = false;

    int opt = getopt(argc, argv, "lnru");
    while (opt != -1) {
        if (opt == 'l') {
            cmp = cmp_pstr_len;
        } else if (opt == 'n') {
            cmp = cmp_pstr_numeric;
        } else if (opt == 'r') {
            reverse = true;
        } else if (opt == 'u') {
            uniq = true;
        } else {
            return 1;
        }

        opt = getopt(argc, argv, "lnru");
    }

    FILE *fp = stdin;
    if (optind < argc) {
        fp = fopen(argv[optind], "r");
        if (fp == NULL) {
            error(1, 0, "cannot access %s", argv[optind]);
        }
    }
    sort_lines(fp, cmp, uniq, reverse);
    fclose(fp);
    return 0;
}
