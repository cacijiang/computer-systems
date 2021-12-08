#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* (Same function from pre-lecture video, copied here for readability)
 * This function prints out the provided array using
 * the provided function that prints out one element of the array.
 */
void print_array(void *arr, size_t nelems, int elem_size_bytes,
                 void(*print_fn)(void *)) {
    for (int i = 0; i < nelems; i++) {
        void *elem_ptr = (char *)arr + i * elem_size_bytes;
        printf("%d: ", i + 1);
        print_fn(elem_ptr);
        printf("\n");
    }
}

/*
 * char *str = __________;
 * Original choices:
 *  A. (char *) ptr
 *  B. *(char **) ptr
 *  C. *(char *) ptr
 *  D. **(char ***) ptr
 *  Why would A or D /not/ work?
 */
void print_string(void *ptr) {
    char *str = *(char **) ptr;
    printf("%s", str);
}

void print_int(void *arr) {
    printf("%d", *(int *)arr);
}


int do_something (char *str) {
  printf("%s\n", str);
  return strlen(str);
}

int main(int argc, char *argv[]) {
    // Print an array of strings
    char *str_array[] = {"aardvark", "beaver", "capybara"};
    size_t n_elems = sizeof(str_array)/sizeof(str_array[0]);
    print_array(str_array, n_elems, sizeof(str_array[0]), print_string);

    // Do something with variables
    int (*func_var)(char *) = do_something;
    char *str = "testing";
    int retval = func_var(str);
    printf("%d\n", retval);
    return 0;
}
