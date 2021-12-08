/* CS 107 assign3
 * Code by Caci Jiang
 *
 * This program mimics the operation of command "tail", which is to prints the final
 * N lines of the input in order. If the user does not specify command-line flag N,
 * it is 10 as default. If there are not enough lines in the input up to N, then all
 * lines will be printed
 */

#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_LINES 100000
#define DEFAULT_NUM_LINES 10

/* This function accepts a file pointer and an integer n which is an optional argument
 * to specify the number of final lines with a default value of 10. This function will
 * create an array to serve as a sliding window of size n, to accommodate the most recent
 * n lines that have been read so far. For example, if the current i-th line exceeds n,
 * it will overwrite (i - n)-th line in the array. As the strings in the array point to
 * the memory in the heap, after printing the last n lines in the file, these heap memory
 * shall be freed.
 */
void print_last_n(FILE *fp, int n) {
    char *line;
    char *line_arr[n];
    int index = 0; // denote the current line number

    while((line = read_line(fp)) != NULL) {
    	if(index >= n) {
	        // Free previous heap memory at the this cell at first
	        // when the line number exceeds n.
	        free(line_arr[index % n]);
	    }
	    line_arr[index % n] = line;
	    index++;
    }

    // Set the starting index to print equal to 0 if the line number is less than n.
    int start_index  = index - n >= 0 ? index - n : 0;
    for(int i = start_index; i < index; i++) {
    	printf("%s\n", line_arr[i % n]);
	    free(line_arr[i % n]);
    }
}

// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

int convert_arg(const char *str, int max) {
    char *end;
    long n = strtol(str, &end, 10);
    if (*end != '\0') {
        error(1, 0, "Invalid number '%s'", str);
    }
    if (n < 1 || n > max) {
        error(1, 0, "%s is not within the acceptable range [%d, %d]", str, 1, max);
    }
    return n;
}

int main(int argc, char *argv[]) {
    int num_lines = DEFAULT_NUM_LINES;

    if (argc > 1 && argv[1][0] == '-') {
        num_lines = convert_arg(argv[1] + 1, MAX_NUM_LINES);
        argv++;
        argc--;
    }

    FILE *fp;
    if (argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            error(1, errno, "cannot access '%s'", argv[1]);
        }
    }
    print_last_n(fp, num_lines);
    fclose(fp);
    return 0;
}
