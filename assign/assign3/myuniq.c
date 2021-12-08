/* CS 107 assign3
 * Code by Caci Jiang
 *
 * This program mimics the operation of command "uniq" with "-c" flag, which will print
 * all the unique lines given a file pointer as well as their corresponding occurrences
 * as prefixes. This program supports no command-line flags.
 */

#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* initial estimate of number of uniq lines
 * resize-as-you-go, add in increments of 100
 */
#define ESTIMATE 100

/* This struct is to record each unique line and its occurrence in the file. */
typedef struct uniq_line {
    char *str; // the string is located in heap memory
    int count;
} uniq_line;

/* This helper function accepts current line, the the array contains all unique lines so far
 * and its current size. The function will return true, if current line has already been kept
 * in the array, and increment its occurrence by 1. Otherwise, it will return false if it is
 * a new unique line.
 */
bool is_visited(char *line, uniq_line *uniq_lines, int nuniq) {
    for(int i = 0; i < nuniq; i++) {
    	if(strcmp(uniq_lines[i].str, line) == 0) {
	        uniq_lines[i].count++;
	        return true;
	    }
    }
    return false;
}

/* This function accepts a file pointer and finally print all the unique lines line by line,
 * with each prefixed by its occurrence in the file. Firstly, this function will allocate heap
 * memory for the array to accommodate the unique_line struct. Then it will read the file line
 * by line. This function will call helper function is_visited to check whether current has been
 * visited. If yes, incremnent its occurrence and free the memory of current line. If not, it
 * will check whether the struct array still has enough capacity. Otherswise, it will enlarge
 * the array size by ESTIMATE. Then, the new unique will be added to the array. Finally, it prints
 * each unique line prefixed by the occurrence. All memorty allocated in the heap shall be freed.
 */
void print_uniq_lines(FILE *fp) {
    char *line;
    int nuniq = 0; // denote the number of unique lines that have been bookkept
    uniq_line *uniq_lines = malloc(sizeof(uniq_line) * ESTIMATE);
    assert(uniq_lines != NULL);
    
    while((line = read_line(fp)) != NULL) {
    	if(!is_visited(line, uniq_lines, nuniq)) {
	        // Check whether the struct array has enough capacity
	        if(nuniq >= ESTIMATE && nuniq % ESTIMATE == 0) {
		        // Enlarge the capacity of the struct array
	    	    uniq_lines = realloc(uniq_lines, sizeof(uniq_line) * (nuniq + ESTIMATE));
		        assert(uniq_lines != NULL);
	        }
	        uniq_lines[nuniq++] = (uniq_line) {line, 1};
	    } else {
	        free(line); // free current line memory as it is not needed any more
	    }
    }

    for(int i = 0; i < nuniq; i++) {
    	printf("%7d %s\n", uniq_lines[i].count, uniq_lines[i].str);
	    free(uniq_lines[i].str); // free the heap memeory occupied by the line string
    }

    free(uniq_lines); // free the heap memory occupied by the struct array
}


// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

int main(int argc, char *argv[]) {
    FILE *fp;

    if (argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            error(1, errno, "cannot access '%s'", argv[1]);
        }
    }
    print_uniq_lines(fp);
    fclose(fp);
    return 0;
}
