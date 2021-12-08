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


int compare_pointers(const void *p, const void *q) {
    return (const char *)p - (const char *)q;
}

int compare_integers(const void *p, const void *q) {
    return *(const int *)p - *(const int *)q;
}

int compare_characters(const void *p, const void *q) {
    return *(const unsigned char *)p - *(const unsigned char *)q;
}

int compare_first_characters(const void *p, const void *q) {
    return **(const char **)p - **(const char **)q;
}

int compare_asymmetric(const void *p, const void *q) {
    return *(const char *)p - **(const char **)q;
}

int cmp_fn(const void *p, const void *q) {
    return *(const char *) p - *(const char *) q;
}

void *gfind_max(void *arr, int n, size_t elemsz, 
    int (*compare_function)(const void *, const void *)) {

    void *pmax = arr;
    for (int i = 1; i < n; i++) {
        void *ith = (char *)arr + i*elemsz;
        if (compare_function(ith, pmax) > 0) {
            pmax = ith;
        }
    }
    return pmax;
}

int main(int argc, char *argv[]) {
    char buf[] = "AppleCake?";
    size_t len = strlen(buf);
    qsort(buf, len, sizeof(buf[0]), cmp_fn);
    printf("call on qsort:%s\n", buf);


    int nums[] = {0x33444433, 0x59111159, 0x67, 0};
    int n = sizeof(nums) / sizeof(nums[0]);

    int max = *(int *)gfind_max(nums, n, sizeof(nums[0]), compare_integers);
    printf("correct call max = %#x\n", max);

    max = *(int *)gfind_max(nums, n, sizeof(nums[0]), compare_characters);
    printf("incorrect call #1 max = %#x\n", max);

    max = *(int *)gfind_max(nums, n, 1, compare_characters);
    printf("incorrect call #2 max = %#x\n", max);

    max = *(int *)gfind_max(nums, n, sizeof(nums[0]), compare_pointers);
    printf("incorrect call #3 max = %#x\n", max);
    return 0;
}
