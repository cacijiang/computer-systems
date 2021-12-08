#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dinky(int val) {
    return val * 2;
}

int binky(int a, int b) {
    return a + dinky(b);
}

int clyde(int arr[], int index) {
    return arr[index];
}

int bigbird(int a, int b) {
    /* Single local variables are likely to end up in registers.
     * The array below will be placed on the stack.
     */

    int arr[] = {1, 2, 3};

    int x = a * 2;
    int y = b - x;
    int z = y * b;
    int q = 10 + z + a;
    int p = q + b;
    int m = p + q + b;
    int n = m + 2;
    return n + clyde(arr, 1);
}

int kermit(int p) {
    return binky(p, -1) * dinky(p);
}

int main(int argc, char *argv[]) {
    printf("This program demonstrates various behaviors of the runtime stack.\n");
    int val = kermit(107);
    val += bigbird(7, 2);
    printf("All done, goodbye!\n");
    return val;
}
