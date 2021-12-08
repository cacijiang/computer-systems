/* File: token.c
 * -------------
 * Implementation of strtok for code study.
 * Read lab2 writeup for more information.
 */

#include <stdio.h>
#include <string.h>

char *musl_strtok(char *s, const char *delimiters) {
    static char *p = NULL;

    if (s == NULL) {
        s = p;
        if (s == NULL) {
            return NULL;
        }
    }

    s += strspn(s, delimiters);
    if (!*s) {
        p = NULL;
        return p;
    }

    p = s + strcspn(s, delimiters);
    if (*p) {
        *(p++) = '\0';
    } else {
        p = NULL;
    }

    return s;
}

void tokenizationLoop(char *input, const char *delimiters) {
    char *token = musl_strtok(input, delimiters);
    while (token != NULL) {
        printf("%s\n", token);
        token = musl_strtok(NULL, delimiters);
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    char *str = "red-green-blue";
    char *first;
    char copy[strlen(str) + 1];
    strcpy(copy, str);

    // (uncomment line below) attempt to tokenize string constant fails at runtime. Why?
    // first = musl_strtok(str, "-");

    // but ok to tokenize copy. Why? What's different?
    first = musl_strtok(copy, "-");  
    printf("First token is %s\n", first);

    // Example tokenization loop
    char path[] = "/afs/ir/class/cs107";
    tokenizationLoop(path, "/");

    return 0;
}
