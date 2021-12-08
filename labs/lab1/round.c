/* File: round.c
 * -------------
 * Code passages to read as part of lab1.
 */

#include <stdbool.h>
#include <stdio.h>

// From lecture
bool is_power_of_2(unsigned int x) {
    return x != 0 && (x & (x - 1)) == 0;
}

/* What is special about powers of 2 that allow rounding via bitwise ops
 * in place of the more expensive multiply/divide used for general case?
 */
unsigned int round_up(unsigned int val, unsigned int mult) {
    if (!is_power_of_2(mult)) {
        return ((val + mult - 1) / mult) * mult;
    } else {
        return (val + mult - 1) & ~(mult - 1);
    }
}

void print_round_up(unsigned int v, unsigned int m) {
    printf("round_up(%u, %u) = %u\n", v, m, round_up(v, m));
}

int main(int argc, char *argv[]) {
    print_round_up(2, 7);
    print_round_up(6, 5);
    print_round_up(100, 32);
    print_round_up(4, 4);
    return 0;
}

