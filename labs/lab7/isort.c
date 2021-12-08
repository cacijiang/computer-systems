/* File: isort.c
 * ------------
 * Insertion sort with 3 different possible swap routines, which differ in
 * where they store temporary data: fixed stack array, var stack array, or alloc/free from heap
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// This version uses a fixed size stack buffer for the temp storage.
void swap_fixedstack(void *a, void *b, size_t sz) {
    char tmp[8];
    memcpy(tmp, a, sz);
    memcpy(a, b, sz);
    memcpy(b, tmp, sz);
}

// This version uses a variable-sized stack buffer for the temp storage.
void swap_varstack(void *a, void *b, size_t sz) {
    char tmp[sz];
    memcpy(tmp, a, sz);
    memcpy(a, b, sz);
    memcpy(b, tmp, sz);
}

// The version uses the heap (malloc/free) for the temp storage.
void swap_heap(void *a, void *b, size_t sz) {
    void *tmp = malloc(sz);
    memcpy(tmp, a, sz);
    memcpy(a, b, sz);
    memcpy(b, tmp, sz);
    free(tmp);
}

long insertion_sort(long arr[], size_t n, void (*swap)(void *, void *, size_t)) {
    long nswaps = 0;
    for (long i = 1; i < n; i++) {
        for (long j = i - 1; j >= 0 && arr[j + 1] < arr[j]; j--) {
            swap(&arr[j + 1], &arr[j], sizeof(arr[j]));
            nswaps++;
        }
    }
    return nswaps;
}

void warmup_lib(int n) {
    // resolve lib routines before counting
    void *p = malloc(n);
    memcpy(p, &n, n);
    free(p);
}

// This is a quick hack to work around a dumb bug in callgrind annotate, please ignore
int swap_fix(int n) {
    int x = 1;
    for (int i = 0; i < n; i++) {
        x *= n;
    }
    return x;
}

/* Configuration code to run different functions */

static struct {
    void (*fn)(void *, void *, size_t);
    char *name;
} trials[] =   
     {{swap_fixedstack, "using fixed-len stack array"}, 
      {swap_varstack, "using variable-len stack array"}, 
      {swap_heap, "in heap"}};

static const int nfns = sizeof(trials) / sizeof(*trials);

int main(int argc, char *argv[]) {
    const int nelems = argc > 1 ? atoi(argv[1]) : 2;
    long unsorted[nelems];
    long arr[nelems];
    warmup_lib(nelems);
    int x = swap_fix(30);

    for (int j = 0; j < nelems; j++) {
        unsorted[j] = nelems - j;
    }

    for(int i = 0; i < nfns; i++) {
         // init array in same configuration for each trial
        memcpy(arr, unsorted, sizeof(unsorted));
        long nswaps = insertion_sort(arr, nelems, trials[i].fn);
        printf("Insertion sort (%zu total swaps), swap stores temp %s\n", nswaps, trials[i].name);
    }
    return x;
}
