/*
 * CS107
 * Lab 4
 *
 * This file shows examples of passing functions as parameters.
 * In particular, it calls the C qsort function to sort arrays of elements
 * using various example comparison functions listed below.
 * See the lab 4 writeup for more information.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct city {
    char *name;
    int zipcode;
} city;


int compare_integers(const void *a, const void *b) {
    int first  = *(const int *)a;
    int second = *(const int *)b;

    if (first < second) {
        return -1;
    } else if (first > second) {
        return 1;
    } else {
        return 0;
    }
}

int city_compare_by_name(const void *a, const void *b) {
    const city *first  = (const city *)a;
    const city *second = (const city *)b;

    return strcmp(first->name, second->name);
}

int city_compare_by_zip(const void *a, const void *b) {
    const city *first  = (const city *)a;
    const city *second = (const city *)b;

    return first->zipcode - second->zipcode;
}

int compare_strings(const void *a, const void *b) {
    const char *first  = *(const char **)a;
    const char *second = *(const char **)b;

    return strcmp(first, second);
}

int compare_letters(const void *a, const void *b) {
    unsigned char first  = *(const char *)a;
    unsigned char second = *(const char *)b;

    return first - second;
}

// This function demos the qsort function on an array of integers.
void sort_integers() {
    int scores[] = {80, 72, 74, 85, 79, 81};
    int nscores = sizeof(scores) / sizeof(scores[0]);

    qsort(scores, nscores, sizeof(scores[0]), compare_integers);
    
    printf("Sort integers: ");
    for (int i = 0; i < nscores; i++) {
        printf("%d ", scores[i]);
    }
    printf("\n");
}

/* This function demos the qsort function on an array of custom structs,
 * sorting the first time by city names, the second time by zipcode.
 */
void sort_cities() {
    city cities[] = {
        {"Stanford", 94305},
        {"Seattle", 98144},
        {"Stevinson", 95374},
        {"Seattle", 98101}
    };
    int ncities = sizeof(cities) / sizeof(cities[0]);

    printf("Sort city structs by name: \n");
    qsort(cities, ncities, sizeof(cities[0]), city_compare_by_name);

    for (int i = 0; i < ncities; i++) {
        printf("{%s,%d} ", cities[i].name, cities[i].zipcode);
    }

    printf("Sort city structs by zipcode: \n");
    qsort(cities, ncities, sizeof(cities[0]), city_compare_by_zip);

    for (int i = 0; i < ncities; i++) {
        printf("{%s,%d} ", cities[i].name, cities[i].zipcode);
    }
    printf("\n");
}

/* This function demos the qsort function on an array of strings,
 * sorting them alphabetically.
 */
void sort_words() {
    char *words[] = {"cherry", "apple", "banana", "peach", "pear", "kiwi"};
    int nwords = sizeof(words) / sizeof(words[0]);

    qsort(words, nwords, sizeof(words[0]), compare_strings);

    printf("Sort strings: ");
    for (int i = 0; i < nwords; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    sort_integers();
    sort_cities();
    sort_words();
    return 0;
}
