/*
 * File: live_generic_stack.c
 * Written by Lisa Yan
 * ---------------------
 * What if we took the generic stack from before and
 * removed the unnecessary struct node?
 * How would things change? How would things not change?
 */

#include <assert.h>
#include <stdbool.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The number of elements to put in our test stack
const size_t TEST_STACK_SIZE = 10;

/* This type is a stack that contains where the head of
 * its linked list of values is located, its element size, and
 * the width (in bytes) of the data type it is storing.
 */
typedef struct stack {
    size_t elem_size_bytes;
    size_t nelems;
    void *top; 
} stack;

/* This function dynamically allocates a new empty
 * stack and returns a pointer to it.
 */
stack *stack_create(size_t elem_size_bytes) {
    stack *s = malloc(sizeof(stack));
    assert(s != NULL);
    s->elem_size_bytes = elem_size_bytes;
    s->nelems = 0;
    s->top = NULL;
    return s;
}

/* This function adds a copy of data to the top of the stack. */
void stack_push(stack *s, const void *data) {
    // node looks like: (8 bytes of next node's address) (data)
    void *new_node = malloc(sizeof(void *) + s->elem_size_bytes);
    assert(new_node != NULL);

    // copy data into the new node
    memcpy((char *) new_node + sizeof(void *), data, s->elem_size_bytes);

    // set new node's next to be top of stack
    *((void **) new_node) = s->top;

    // then set top of stack to be new node
    s->top = new_node;
    s->nelems++;
}

/* This function removes and returns the data at the top of the stack.
 * If the stack is empty, this function throws an error. */
void stack_pop(stack *s, void *addr) {
    if (s->nelems == 0) {
        error(1, 0, "Cannot pop from empty stack");
    }

    void *n = s->top;
    memcpy(addr, (char *) n + sizeof(void *), s->elem_size_bytes);

    // rewire
    s->top = *(void **) n;
    free(n);
    s->nelems--;
}

void stack_delete(stack *s) {
    char temp[s->elem_size_bytes];
    while (s->nelems) {
        stack_pop(s, temp);
    }
    free(s);
}

/************************** main **************************/
int main(int argc, char *argv[]) {
    int arr[] = {1, 2, 3, 4};
    void *ptr = arr;
    int elt_1 = * (int *) ((char *) ptr + sizeof(int));
    printf("elt 1: %d\n", elt_1);
    
    /* int stack */
    stack *intstack = stack_create(sizeof(int));
    for (int i = 0; i < TEST_STACK_SIZE; i++) {
        stack_push(intstack, &i); 
    }

    // Pop off all elements
    int popped_int;
    while (intstack->nelems > 0) {
        stack_pop(intstack, &popped_int);
        printf("%d\n", popped_int);
    }
    printf("\n");

    // push one more to check to see if our wiring
    // works when we removed all of the elements
    int num = 7;
    stack_push(intstack, &num);
    stack_pop(intstack, &popped_int);
    printf("%d\n", popped_int);

    // clean up
    free(intstack);
    printf("\n");

    /* string stack */
    // Try a stack of the command line arguments
    stack *string_stack = stack_create(sizeof(argv[0]));
    for (int i = 1; i < argc; i++) {
        stack_push(string_stack, argv + i); 
    }

    char *next_arg;
    while (string_stack->nelems > 0) {
        stack_pop(string_stack, &next_arg);
        printf("%s\n", next_arg);
    }
    printf("\n");

    // add read only string to see if it works
    char *s = "hello";
    stack_push(string_stack ,&s);
    stack_pop(string_stack, &next_arg);
    printf("%s\n", next_arg);

    // clean up
    free(string_stack);
    
    return 0;
}

