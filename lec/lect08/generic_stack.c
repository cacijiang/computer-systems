/*
 * CS107
 * Lecture 8
 *
 * This program shows how we can create a generic stack
 * variable type to represent a Stack data structure,
 * and how we can implement functions to push and pop
 * from that stack.
 */

#include <assert.h>
#include <error.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The number of elements to put in our test stack
const int TEST_STACK_SIZE = 10;

/* This type is a single node in our linked list of ints,
 * that stores a pointer to a single value and where the next
 * node lives.
 */
typedef struct node {
    struct node *next;
    void *data;
} node;

/* This type is a stack that contains where the head of
 * its linked list of values is located, its element size, and
 * the width (in bytes) of the data type it is storing.
 */
typedef struct stack {
    int elem_size_bytes;
    int nelems;
    node *top; 
} stack;

/* This function dynamically allocates a new empty
 * stack and returns a pointer to it.
 */
stack *stack_create(int elem_size_bytes) {
    stack *s = malloc(sizeof(stack));
    assert(s != NULL);
    s->elem_size_bytes = elem_size_bytes;
    s->nelems = 0;
    s->top = NULL;
    return s;
}

/* This function adds a copy of data to the top of the stack. */
void stack_push(stack *s, const void *data) {
    node *new_node = malloc(sizeof(node));
    assert(new_node != NULL);

    // Make a copy of data and save a pointer to it
    new_node->data = malloc(s->elem_size_bytes);
    assert(new_node->data != NULL);
    memcpy(new_node->data, data, s->elem_size_bytes);

    new_node->next = s->top;
    s->top = new_node;
    s->nelems++;
}

/* This function removes and returns the data at the top of the stack.
 * If the stack is empty, this function throws an error. */
void stack_pop(stack *s, void *addr) {
    if (s->nelems == 0) {
        error(1, 0, "Cannot pop from empty stack");
    }
    node *n = s->top;
    memcpy(addr, n->data, s->elem_size_bytes);
    // rewire
    s->top = n->next; 

    free(n->data);
    free(n);
    s->nelems--;
}

int main(int argc, char *argv[]) {
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

    // push one more to check to see if our wiring
    // works when we removed all of the elements
    int num = 7;
    stack_push(intstack, &num);
    stack_pop(intstack, &popped_int);
    printf("%d\n", popped_int);

    // clean up
    free(intstack);

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

    // add read only string to see if it works
    char *s = "hello";
    stack_push(string_stack ,&s);
    stack_pop(string_stack, &next_arg);
    printf("%s\n", next_arg);

    // clean up
    free(string_stack);
    
    return 0;
}

