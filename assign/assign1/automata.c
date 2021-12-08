#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define INVERTED_BOX "\e[7m \e[0m"

/* uncomment below to swap in "+" as definition of LIVE_STR if your
 * terminal doesn't play nice with inverted box
 */
#define LIVE_STR INVERTED_BOX
// #define LIVE_STR "+"
#define EMPTY_STR " "
#define DEFAULT_GEN_0 (1L << 32)
#define N_GENERATIONS 32

/* Params: (unsigned int) neighbor, (unsigned char) ruleset
 * Return: (usigne long) next_state
 * Given the neighborhood combination at the current generation, generate next
 * state at the targeted position by applying the provided ruleset. In the ruleset,
 * check the combination by shifting to the position denoted by "neighbor".
 * */
unsigned long get_next_state(unsigned int neighbor, unsigned char ruleset) {
    if(ruleset & (1L << neighbor)) {
        return 1UL;
    } else {
        return 0UL;
    }
}

/* Params: (unsigned long) cur_gen, (unsigned char) ruleset
 * Return: (unsigned long) next_gen
 * This function returns the next generation based on current generation
 * and the input ruleset, which applys the bitwise operation from left to
 * right. At each iteration, slice a three-bit combination to get the next
 * state.
 * */
unsigned long advance(unsigned long cur_gen, unsigned char ruleset) {
    unsigned long next_gen = 0;
    for(int i = 63; i >= 0; i--) {
        unsigned int left = (i == 63) ? 0 : ((cur_gen >> (i + 1)) & 1L);
	    left <<= 2;
	    unsigned int mid = ((cur_gen >> i) & 1L);
	    mid <<= 1;
	    unsigned int right = (i == 0) ? 0 : ((cur_gen >> (i - 1)) & 1L);
	    // get three-bit combination
	    unsigned int neighbor = left | mid | right;
	    unsigned long next_state = get_next_state(neighbor, ruleset) << i;
	    next_gen |= next_state;
    }
    return next_gen;
}

/* Params: (unsigned long) gen
 * This function prints the current generation.
 * */
void draw_generation(unsigned long gen) {
    for(int i = 63; i >= 0; i--) {
    	if((gen >> i) & 1UL) {
            printf(LIVE_STR);
	    } else {
	        printf(EMPTY_STR);
	    }
    }
    printf("\n");
}




// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------


unsigned long convert_arg(const char *str, unsigned long low, unsigned long high, const char *argname) {
    char *end;
    unsigned long n = strtoul(str, &end, 0);
    if (*end != '\0') {
        error(1, 0, "Invalid number '%s' for %s", str, argname);
    }
    if (n < low || n > high || errno == ERANGE) {
        error(1, 0, "%s %s is not in range [%#lx, %#lx]", argname, str, low, high);
    }
    return n;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        error(1, 0, "Missing argument. Please supply ruleset and optional initial generation.");
    }

    unsigned long gen_0 = DEFAULT_GEN_0;
    unsigned char ruleset = convert_arg(argv[1], 0, UCHAR_MAX, "ruleset");
    if (argc > 2) {
        gen_0 = convert_arg(argv[2], 0, ULONG_MAX, "initial generation");
    }

    if (ruleset == 0) {
        draw_generation(gen_0);
    } else {
        unsigned long cur_gen = gen_0;
        for (int i = 0; i < N_GENERATIONS; i++) {
            draw_generation(cur_gen);
            unsigned long next_gen = advance(cur_gen, ruleset);
            if (next_gen == cur_gen) {
                break;
            }
            cur_gen = next_gen;
        }
    }
    return 0;
}
