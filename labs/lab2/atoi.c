/* File: atoi.c
 * ------------
 * Implementation of atoi for code study.
 * Read lab2 writeup for more information.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>


int musl_atoi(const char *s) {
    while (isspace(*s)) {
        s++;
    }

    bool negative = false;
    switch (*s) {
        case '-':
            negative = true;
        case '+':
            s++;
    }
    
    /* Compute as a negative number to avoid overflow on INT_MIN */
    int number = 0;
    while (isdigit(*s)) {
        number = 10 * number - (*(s++) - '0');
    }
    
    return negative ? number : -number;
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("musl_atoi(\"%s\") = %d\n", argv[i], musl_atoi(argv[i]));
    }
    return 0;
}
