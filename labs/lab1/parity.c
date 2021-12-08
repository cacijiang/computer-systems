/* File: parity.c
 * --------------
 * Read lab1 writeup for more information.
 */

#include <error.h>
#include <stdio.h>
#include <stdlib.h>


int compute_parity(int x) {
    int result;

    while (x != 0) {
         // invert result if lsb is on
        result ^= (x & 1);
        
        // shift bits down
        x = x >> 1;         
    }
    return result;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        error(1, 0, "missing argument");
    }
    int val = atoi(argv[1]);
    
    /* Note for below: a "ternary" operator has the form EXPRESSION ? IF_TRUE : IF_FALSE
     * If the expression is true, it evaluates to the value in IF_TRUE.  Otherwise, it
     * evaluates to the value in IF_FALSE.  For instance, below, if compute_parity(val)
     * is true, the third parameter will evaluate to "odd".  Otherwise, it will be "even".
     */
    printf("%d has %s parity\n", val, compute_parity(val) ? "odd" : "even");   
    return 0;
}
