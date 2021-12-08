#include <stdio.h>

#define N_ELEMS 10
#define SUM (N_ELEMS * (N_ELEMS + 1)) / 2

void wait_for_user() {
    char line[80];
    printf("\nPress ENTER to continue: ");

    // Get user input via the command line (stdin)
    if (fgets(line, sizeof(line), stdin)) {
        return;
    }
}

void print_array(char *label, int arr[], int n) {
    printf("During %s, arr = { %d", label, arr[0]);
    for (int i = 1; i < n; i++) {
        printf(", %d", arr[i]);
    }
    printf(" }\n");
}

void init_array() {
    int nums[N_ELEMS];
   
    for (int i = 0; i < N_ELEMS; i++) {
        nums[i] = i+1;
    }
    print_array("init_array", nums, N_ELEMS);
}

int sum_array() {
    int nums[N_ELEMS];
    int sum = 0;
    
    for (int i = 0; i < N_ELEMS; i++) {
        sum += nums[i];
    }
    print_array(" sum_array", nums, N_ELEMS);
    return sum;
}


int main(int argc, char *argv[]) {
    int result;

    printf("Call init_array() then sum_array(), nothing in between.\n");
    init_array();
    result = sum_array();
    printf("Channeling %s\n", (result == SUM ? "worked." : "FAILED!"));

    wait_for_user();

    printf("\nAgain, but this time with a debug printf added in between.\n");
    init_array();
    printf("This is an innocent printf statement. It's not my fault! (or is it?)\n");
    result = sum_array();
    printf("Channeling %s\n", (result == SUM ? "worked." : "FAILED!"));
    return 0;
}
