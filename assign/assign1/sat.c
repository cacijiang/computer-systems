#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* Params: (int) bitwidth
 * Return: max signed value given the input bit range
 * */
long signed_max(int bitwidth) {
    return (1L << (bitwidth - 1)) - 1;
}

/* Params: (int) bitwidth
 * Return: min signed value given the input bit range
 * */
long signed_min(int bitwidth) {
    return ~0L << (bitwidth - 1);
}

/* Params: (long) operand1, (long) operand2, (int) bitwidth
 * Return: (long) sum of operand1 and operand2
 * If the sum of operand1 and operand2 exceeds the max signed value allowed by
 * the bitwidth, return max signed value. If the sum exceeds the min signed value
 * given the bitwidth, return min signed value. Otherwise, just return the sum.
 * */
long sat_add(long operand1, long operand2, int bitwidth) {
    long res = operand1 + operand2;
    long mask = 1L << (bitwidth - 1);
    // Overflow: the sum of two positive numbers exceeds max signed value.
    if(~(operand1 & mask) & ~(operand2 & mask) & (res & mask)) {
    	return signed_max(bitwidth);
    }
    // Underflow: the sum of two negative numbers exceeds min signed value.
    if((operand1 & mask) & (operand2 & mask) & ~(res & mask)) {
    	return signed_min(bitwidth);
    }
    return res;
}




// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------


long convert_arg(const char *str, long low, long high, const char *argname) {
    char *end;
    long n = strtol(str, &end, 0);
    if (*end != '\0') {
        error(1, 0, "Invalid number '%s'", str);
    }
    if (n < low || n > high || errno == ERANGE) {
        error(1, 0, "Illegal value %s. %s must be in range [%ld, %ld]", str, argname, low, high);
    }
    return n;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        error(1, 0, "Missing argument. Please specify the bitwidth.");
    }
    
    int bitwidth = convert_arg(argv[1], 4, sizeof(long) * CHAR_BIT, "Bitwidth");
    long min = signed_min(bitwidth);
    long max = signed_max(bitwidth);

    if (argc < 4)  {
        printf("%d-bit signed integer range\n", bitwidth);
        printf("min: %ld   %#018lx\n", min, min);
        printf("max:  %ld   %#018lx\n", max, max);
    } else {
        long sum = convert_arg(argv[2], min, max, "Operand");
        printf("%ld ", sum);
        for (int i = 3; i < argc; i++) {
            long next = convert_arg(argv[i], min, max, "Operand");
            printf("+ %ld ", next);
            sum = sat_add(sum, next, bitwidth);
        }
        printf("= %ld\n", sum);   
    }
    return 0;
}

