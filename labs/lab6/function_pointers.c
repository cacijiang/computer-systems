#include <string.h>
#include <stdio.h>

void *gfind_max(void *arr, size_t n, int elemsz,
                int (*compare_function)(const void *, const void *)) {
    void *pmax = arr;
    for (int i = 1; i < n; i++) {
        void *ith = (char *)arr + i * elemsz;
        if (compare_function(ith, pmax) > 0) {
            pmax = ith;
        }
    }
    return pmax;
}

int cmp_alpha(const void *p, const void *q) {
    const char *first = *(const char **)p;
    const char *second = *(const char **)q;
    return strcmp(first, second);
}

int main(int argc, char *argv[]) {
    char **max = gfind_max(argv + 1, argc - 1,
                           sizeof(argv[0]), cmp_alpha);
    printf("max = %s\n", *max);
    return 0;
}
