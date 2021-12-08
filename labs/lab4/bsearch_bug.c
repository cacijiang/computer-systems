/*
 * CS107
 * Lab 4
 *
 * This file shows examples of using comparison functions when searching
 * and sorting.  See the lab writeup for more information.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int compare_first_characters(const void *p, const void *q) {
    return **(const char **)p - **(const char **)q;
}

int compare_asymmetric(const void *p, const void *q) {
    return *(const char *)p - **(const char **)q;
}


int main(int argc, char *argv[]) {
    char *words[] = {"red", "blue", "green", "yellow"};
    int n = sizeof(words) / sizeof(*words);
    char *key = "g";

    /* sort and search should use same comparison function.  Why are
     * they different here?  Try to figure it out!
     */
    qsort(words, n, sizeof(words[0]), compare_first_characters);

    printf("Sorted by first letter: \n");
    for (int i = 0; i < n; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");

    printf("Looking for a word that starts with %s: \n", key);
    char **found = bsearch(key, words, n, sizeof(words[0]), compare_first_characters);
    printf("found %s\n", found ? *found : "none");
    return 0;
}
