/* CS 107 assign4
 * Code by Caci Jiang
 *
 * This program is designed based on Apple's bsearch. If the key is not found, it will insert
 * the key into the proper position in the sorted array. 
 */
#include "samples/prototypes.h"
#include "string.h"

/* This function is designed upon Apple's bsearch, which accepts a pointer to the search key,
 * a pointer to the start of the array, a pointer to the array size, the element size, and a
 * client-defined compare function. So, this function will perform binary search to find the
 * key in the sorted array. If the key is not found, it will insert the key into the array and
 * update the array size accordingly. Finally, return the pointer to the key.
 */
void *binsert(const void *key, void *base, size_t *p_nelem, size_t width,
              int (*compar)(const void *, const void *)) {
    void *last = (char *) base + (*p_nelem - 1) * width; // get the pointer to the last element
    for (size_t nremain = *p_nelem; nremain != 0; nremain >>= 1) {
        void *p = (char *)base + (nremain >> 1) * width;
        int sign = compar(key, p);
        if (sign == 0) {
            return p;
        } 
        if (sign > 0) {
            base = (char *)p + width;
            nremain--;
        }
    }
    size_t nbytes = (char *)last - (char *)base + width;
    memmove((char *) base + width, base, nbytes);
    memcpy(base, key, width);
    (*p_nelem)++;
    return base;
}
