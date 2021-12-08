/* File: copy.c
 * ------------
 * This program compares the performance of different data copying techniques.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The size of a chunk to copy per iteration of the copy_memloop function loop
const size_t MEMLOOP_CHUNK_BYTES = 8;

void *copy_charloop(void *dst, const void *src, size_t nbytes) {
    // use char * to copy by char
    char *d = dst;
    const char *s = src;

    for (long i = 0; i < nbytes; i++) {
        d[i] = s[i];
    }
    return dst;
}

void *copy_intloop(void *dst, const void *src, size_t nbytes) {
    // use int * to copy by int
    int *d = dst;
    const int *s = src;
    size_t n_iterations = nbytes / sizeof(*d);

    for (long i = 0; i < n_iterations; i++) {
        d[i] = s[i];
    }
    return dst;
}

void *copy_longloop(void *dst, const void *src, size_t nbytes) {
    // use long * to copy by long
    long *d = dst;
    const long *s = src;
    size_t n_iterations = nbytes / sizeof(*d);

    for (long i = 0; i < n_iterations; i++) {
        d[i] = s[i];
    }
    return dst;
}

void *copy_memloop(void *dst, const void *src, size_t nbytes) {
    size_t n_iterations = nbytes / MEMLOOP_CHUNK_BYTES;

    // Copy one chunk at a time
    for (long i = 0; i < n_iterations; i++) {
        memcpy((char *)dst + i * MEMLOOP_CHUNK_BYTES,
               (char *)src + i * MEMLOOP_CHUNK_BYTES, MEMLOOP_CHUNK_BYTES);
    }
    return dst;
}

void *copy_memcpy(void *dst, const void *src, size_t nbytes) {
    return memcpy(dst, src, nbytes);
}

void *copy_memmove(void *dst, const void *src, size_t nbytes) {
    return memmove(dst, src, nbytes);
}

/* Configuration code to run different functions */

#define DATA_SIZE (1 << 10)

static struct {
   void *(*fn)(void *, const void *, size_t);
   char *msg;
   size_t val;
} trials[] = 
    {{copy_charloop, "loop char (%zu iterations, loop body single char assignment dst[i] = src[i], movb)", DATA_SIZE}, 
    {copy_intloop, "loop int (%zu iterations, loop body single int assignment dst[i] = src[i], movl)", DATA_SIZE / sizeof(int)}, 
    {copy_longloop, "loop long (%zu iterations, loop body single long assignment dst[i] = src[i], movq)", DATA_SIZE / sizeof(long)}, 
    {copy_memloop, "loop memcpy (%zu iterations, loop body memcpy(dst, src, 8)", DATA_SIZE / sizeof(long)}, 
    {copy_memmove, "no loop, single call on entire block, memmove(dst, src, %zu)", DATA_SIZE}, 
    {copy_memcpy, "no loop, single call on entire block, memcpy(dst, src, %zu)", DATA_SIZE}};
static const int nfns = sizeof(trials) / sizeof(*trials);


void warmup_lib(void *p, void *q, int n) {
    // resolve lib routines before counting
    memcpy(p, q, n / 2);
    memmove(q, p, n / 2);
}

int main(int argc, char *argv[]) {
    char src[DATA_SIZE];
    char dst[DATA_SIZE];
    warmup_lib(src, dst, 2);

    for(int i = 0; i < nfns; i++) {
        trials[i].fn(dst, src, sizeof(src));
        printf(trials[i].msg, trials[i].val);
        printf("\n");
   }
   return 0;
}
