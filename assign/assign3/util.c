/* CS 107 assign3
 * Code by Caci Jiang
 *
 * This program is designed to read a line given a pointer of a file, and return
 * that line. It can be called in a loop until it reaches EOF. Each time it will
 * read a line and move the file pointer to the next line.
 */

#include "samples/prototypes.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* initial allocation will be for min size, if not big enough, doubles
 * to 64, then 128, then 256, etc. as needed to accommodate the entire line
 * resize-as-you-go, doubling each time.
 */
#define MINIMUM_SIZE 32

/* This function accepts a file pointer and return one line read at the given file
 * pointer. To implement this, by using fgets, this function will allocate the
 * heap memory for a buffer array and write the current line into the buffer array.
 * The size of the buffer array can be dynamically enlarged if the line is too long.
 */
char *read_line(FILE *fp) {
    size_t buflen = sizeof(char) * MINIMUM_SIZE;
    char *buf  = malloc(buflen);
    assert(buf != NULL);
    char *first_char = fgets(buf, buflen, fp);
    // Directly return NULL if no characters to be read
    if(first_char == NULL) {
    	free(buf);
	    return NULL;
    }

    // Continue reading the rest characters in the line by doubling the size of buffer
    // array.
    while(strchr(buf, '\n') == NULL && first_char != NULL) {
	    buflen *= 2;
    	buf = realloc(buf, buflen);
	    assert(buf != NULL);
	    // Writing is started from where we stopped last time, so the buffer pointer
	    // should be moved.
	    first_char = fgets(buf + strlen(buf), buflen - (buflen / 2), fp);	
    }

    // Replace the newline character by null terminator.
    char *newline_char  = strchr(buf, '\n');
    if(newline_char != NULL) {
	    *newline_char = '\0';
    }
    return buf;
}
