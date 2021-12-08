/* File: trials.c
 * --------------
 * Various ways of computing 2^power or is-power-of-2 to be observed for instruction count
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


// Use the built in library function
long two_to_power_A(unsigned int exp) {
     return pow(2, exp);
}

// is specialized 2^exp less work than general pow()?
long two_to_power_B(unsigned int exp) {
    return exp2(exp);
}

// loop to repeatedly multiply by 2, iterates power # of times
long two_to_power_C(unsigned int exp) {
    long result = 1;
    for (int i = 1; i <= exp; i++) {
        result *= 2;
    }
    return result;
}

// base is known to be 2, simple bit shift in binary is perfect!
long two_to_power_D(unsigned int exp) {
    return 1L << exp;
}



// Some different ways to report whether an int is a power of 2

bool is_power_A(int num) {
    // generate successive powers of two, compare to num for equality
    int n_bits = sizeof(int) * CHAR_BIT;
    for (int i = 0; i < n_bits; i++) {
        if (num == (1 << i)) {
            return true;
        }
    }
    return false;
}

bool is_power_B(int num) {
    // convert to double, use math log2 function, then see if whole-numbered
    double val = log2(num);
    return val == floor(val);
}

bool is_power_C(int num) {
    // popcount built-in counts "on" bits, powers of 2 have only one bit on
    return __builtin_popcount(num) == 1;
}

bool is_power_D(int num) {
    /* switch statement to match powers of two.
     * what assembly is generated for switch statement like this?
     * disassemble to see!
     */
    switch (num) {
        case 1: case 2: case 4: case 8:
        case 0x10: case 0x20: case 0x40: case 0x80:
        case 0x100: case 0x200: case 0x400: case 0x800:
        case 0x1000: case 0x2000: case 0x4000: case 0x8000:
        case 0x10000: case 0x20000: case 0x40000: case 0x80000:
        case 0x100000: case 0x200000: case 0x400000: case 0x800000:
        case 0x1000000: case 0x2000000: case 0x4000000: case 0x8000000:
        case 0x10000000: case 0x20000000: case 0x40000000: case 0x80000000:
            return true;
        default:
            return false;
    }
}

bool is_power_E(int num) {
    /* leverage bitwise identities to find numbers with only one bit on
     * Remember where we saw this previously?
     */
    return num && ((num & (num - 1)) == 0);
}


/* Configuration code to run different functions */

static struct { 
    long (*fn)(unsigned int); 
    char *descr; 
} pfns[] = 
   {{two_to_power_A, "calls pow()"}, 
    {two_to_power_B, "calls exp2()"},
    {two_to_power_C, "multiply loop",}, 
    {two_to_power_D, "bit shift"}};
static const int npfns = sizeof(pfns) / sizeof(*pfns);

static struct { 
    bool (*fn)(int); 
    char *descr; 
} ifns[] =
   {{is_power_A, "multiply loop"}, 
    {is_power_B, "calls log2"},
    {is_power_C, "popcount == 1"}, 
    {is_power_D, "lookup table, switch"},
    {is_power_E, "recognize bitwise/numeric identity"}};
static const int nifns = sizeof(ifns) / sizeof(*ifns);


double warmup_lib(double d) {
    // resolve lib before counting
    return floorf(pow(exp2(d), log2f(d)));
}

int main(int argc, char *argv[]) {
    warmup_lib(rand());

    long result = 0;
    int input = rand();

    for (int i = 0; i < npfns; i++) {
        result += pfns[i].fn(input % 60);
    }

    for (int i = 0; i < nifns; i++) {
        result += ifns[i].fn(input);
    }
    
    return result == 0;
}

