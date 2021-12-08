#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

// ------- DO NOT EDIT ANY CODE IN THIS FILE -------

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

    char *line;
    int n = 1;
    while ((line = read_line(fp)) != NULL) {
        if (*line != '\0') {
            printf("%6d  %s", n++, line);
        }
        printf("\n");
        free(line);
    }
    fclose(fp);
    return 0;
}
