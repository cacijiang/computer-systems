#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


int myfn(long param) {
    int arr[] = {1, 2, 3, param, 0x4567, INT_MIN, strlen("Stanford University")};
    int count = sizeof(arr) / sizeof(arr[0]);

    int total = arr[param];
    int squared = total * total;
    return squared + count;
}

int main(int argc, char *argv[]) {
    int val = myfn(3);
    return val;
}

