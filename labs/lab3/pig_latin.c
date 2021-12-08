/* Lab 3
 * 
 * This program converts text to pig latin as an example of
 * how to use dynamic memory allocation on the heap with malloc,
 * realloc and free.
 *
 * If there are no additional command-line arguments, the program
 * prints out tests of converting individual words to pig latin.
 * If there are additional command-line arguments, the program
 * concatenates the pig latin versions of them and prints out the
 * resulting string.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define LOWERCASE_VOWELS "aeiou"
#define LOWERCASE_ALPHABET "abcdefghijklmnopqrstuvwxyz"

/* Prototypes for pig latin helper functions */
void pig_way(char *out, const char *in);
void pig_regular(char *out, const char *in);

/* Function: pig_latin
 * --------------------------
 * This function returns a pig-latinified version of the in string.
 * It is the caller's responsibility to free the returned string,
 * which is allocated on the heap.
 * Simplified pig latin rules are:
 * - if the word starts with a vowel, append "way"
 * - otherwise, move all initial consonants to the end, and append "ay"
 *
 * This function assumes that the provided word is lowercase.
 * If the word begins with non-alphabetic characters, this function returns NULL.
 */
char *pig_latin(const char *in) {
    // If the word starts with non-alphabetic characters, we can't translate
    if (strcspn(in, LOWERCASE_ALPHABET) > 0) {
        return NULL;
    }

    char *out = NULL;

    // If the word starts with a vowel, add "way"
    if (strchr(LOWERCASE_VOWELS, in[0]) != NULL) {
        int out_len = strlen(in) + strlen("way");
        out = malloc(sizeof(char) * (out_len + 1)); // null terminator
        assert(out != NULL);

        pig_way(out, in);
    } else {
        // Otherwise, move all initial consonants to the end, and append "ay"
        int out_len = strlen(in) + strlen("ay");
        out = malloc(sizeof(char) * (out_len + 1));
        assert(out != NULL);

        pig_regular(out, in);
    }

    return out;
}

/* pig_cat
 * --------------------------
 * This function returns a concatenation of the pig-latinified versions of 
 * all provided strings in strs, in the same order as they appear,
 * with spaces in-between each conversion.
 * It is the caller's responsibility to free the returned string,
 * which is allocated on the heap.
 */
char *pig_cat(char *strs[], int num) {
    if (num == 0) {
        return NULL;
    }

    // Initial string starts the sentence
    char *concat = pig_latin(strs[0]);

    // If the first word can't be translated, just make a copy as-is on the
    // heap to enlarge later
    if (concat == NULL) {
        concat = strdup(strs[0]);
        assert(concat != NULL);
    }

    /* For each additional word, convert to pig latin, reallocate to
     * get more space, and concatenate.
     */
    for (int i = 1; i < num; i++) {
        char *pig_word = pig_latin(strs[i]);

        // If the word can't be translated, just make a copy as-is on the heap
        if (strlen(pig_word) == 0) {
            pig_word = strdup(strs[i]);
            assert(pig_word != NULL);
        }
        
        // realloc
        int concat_len = strlen(concat) + strlen(pig_word) + strlen(" ") + 1;
        concat = realloc(concat, concat_len);
        assert(concat != NULL);

        // append new string to end
        strcat(concat, " ");
        strcat(concat, pig_word);

        free(pig_word);
    }

    return concat;
}
/**********  Pig latin helper function implementations  ******************/

/* Function: pig_way
 * --------------------------
 * This function performs the "way" case of pig latin on the in string, 
 * storing the output in the out buffer.  The "way" case is where we copy in
 * the original word and append "way".
 */
void pig_way(char *out, const char *in) {
    strcpy(out, in);
    strcat(out, "way");
}

/* Function: pig_regular
 * --------------------------
 * This function performs the "regular" case of pig latin on the in string, 
 * storing the output in the out buffer.  The "regular" case is where we move
 * all initial consonants to the end of the word and append "ay".
 */
void pig_regular(char *out, const char *in) {
    int n = strcspn(in, LOWERCASE_VOWELS);
    strcpy(out, in + n);
    strncat(out, in, n);
    strcat(out, "ay");
}

/************************************ main ************************************/

/* Function: test_pig_latin
 * --------------------------
 * This function runs test cases on pig_latin for various individual words,
 * and prints out the result for each.
 */
void test_pig_latin() {
    char *testing[] = {
        "pig",
        "simple",
        "party",
        "omelet",
        "explain",
        "anteater",
        "grouchy",
        "desk"
    };

    int n_elems = sizeof(testing) / sizeof(char *);
    
    for (int i = 0; i < n_elems; i++) {
        char *pig_word = pig_latin(testing[i]);
        printf("%s -> %s\n", testing[i], pig_word);
        free(pig_word);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // Test translating individual words
        test_pig_latin();
    } else {
        // Test translating the command line arguments
        char *concat = pig_cat(argv + 1, argc - 1);
        printf("%s\n", concat);
        free(concat);
    }

    return 0;
}
