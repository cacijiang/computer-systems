/* CS107 Lecture 1
 * Lisa Yan
 * ----
 * This program prints out information about
 * its received command-line arguments.
 */

#include <stdio.h>
#include <string.h>

void doSomething(char *str) {
    str[0] = 'c';
    printf("%s\n", str);
}

int main(int argc, char *argv[]) {
    const char *haystack = "cant stop wont stop but traffic stop";
    const char *needle = "stop";
    int tokens_len = 0;
    char *tokens[100];
    printf("&haystack[5]: %p\n", &haystack[5]);
    int needle_len = strlen(needle);
    while(strstr(haystack, needle) != NULL) {
        haystack = strstr(haystack, needle);
        tokens[tokens_len] = (char *) &haystack[0];
        tokens_len++;
        haystack += needle_len;
    }
    printf("tokens_len: %d\n", tokens_len);
    for(int i = 0; i < tokens_len; i++) {
        printf("i: %d, %p\n", i, tokens[i]);
    }


    /*printf("This program received %d argument(s)\n", argc);
    
    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    char myString[6] = "hello";
    // strcpy(myString, "hello");
    doSomething(myString);
    printf("%s\n", myString);*/
    return 0;
}
