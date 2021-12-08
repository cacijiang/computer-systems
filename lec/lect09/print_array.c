/* CS107
 * Lecture 9
 *
 * This file shows how to implement a generic
 * array printing function that uses void * pointers
 * and function pointers.
 */
#include<stdio.h>
#include<stdlib.h>

/* This struct stores information about temperature info (in Fahrenheit)
 * for a given month, including average low and high temps.
 */
typedef struct month_temp_info {
    char *month;
    int avg_high_f;
    int avg_low_f;
} month_temp_info;

/* This function prints out the provided array using
 * the provided function that prints out one element of the array.
 */
void print_array(void *arr, size_t nelems, int elem_size_bytes,
                 void(*print_fn)(void *)) {
    // TODO
}

void print_int(void *arr) {
    printf("%d", *(int *)arr);
}

void print_long(void *arr) {
    printf("%ld", *(long *)arr);
}

void print_temp_info(void *arr) {
    month_temp_info info = *(month_temp_info *)arr;
    printf("In %s, the average high was %doF, and the average low was %doF",
           info.month, info.avg_high_f, info.avg_low_f);
}

int main(int argc, char *argv[]) {
    // Print an array of ints
    int i_array[] = {0, 1, 2, 3, 4, 5};
    size_t i_nelems = sizeof(i_array) / sizeof(i_array[0]);
    print_array(i_array, i_nelems, sizeof(i_array[0]), print_int);
    printf("\n");

    // Print an array of longs
    long l_array[] = {0, 10, 20, 30, 40, 50};
    size_t l_nelems = sizeof(l_array) / sizeof(l_array[0]);
    print_array(l_array, l_nelems, sizeof(l_array[0]), print_long);
    printf("\n");

    // Print an array of temp info structs
    // data from https://www.usclimatedata.com/climate/winters/california/united-states/usca1252
    month_temp_info month_temps[3];
    month_temps[0] = (month_temp_info){"January", 56, 38};
    month_temps[1] = (month_temp_info){"April", 75, 49};
    month_temps[2] = (month_temp_info){"June", 92, 59};
    size_t temp_nelems = sizeof(month_temps) / sizeof(month_temps[0]);
    print_array(month_temps, temp_nelems, sizeof(month_temps[0]), print_temp_info);

    return 0;
}
