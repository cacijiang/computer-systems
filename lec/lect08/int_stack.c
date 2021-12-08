/*
 * CS107
 * Lecture 8
 *
 * This program shows how we can create an int_stack
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
 * that stores a single integer value and where the next
 * node lives.
 */
typedef struct int_node {
    struct int_node *next;
    int data;
} int_node;

/* This type is a stack that contains where the head of
 * its linked list of values is located, and its size.
 */
typedef struct int_stack {
    int nelems;
    int_node *top; 
} int_stack;

/* This function dynamically allocates a new empty
 * stack and returns a pointer to it.
 */
int_stack *int_stack_create() {
    int_stack *s = malloc(sizeof(int_stack));
    assert(s != NULL);
    s->nelems = 0;
    s->top = NULL;
    return s;
}

/* This function adds a copy of data to the top of the stack. */
void int_stack_push(int_stack *s, int data) {
    int_node *new_node = malloc(sizeof(int_node));
    assert(new_node != NULL);
    new_node->data = data;

    new_node->next = s->top;
    s->top = new_node;
    s->nelems++;
}

/* This function removes and returns the data at the top of the stack. 
 * If the stack is empty, this function throws an error. */
int int_stack_pop(int_stack *s) {
    if (s->nelems == 0) {
        error(1, 0, "Cannot pop from empty stack"); 
    }
    int_node *n = s->top;
    int value = n->data;

    // rewire
    s->top = n->next; 

    free(n);
    s->nelems--;

    return value;
}

int main(int argc, char *argv[]) {
    int_stack *intstack = int_stack_create();
    for (int i = 0; i < TEST_STACK_SIZE; i++) {
        int_stack_push(intstack, i); 
    }

    // Pop off all elements
    while (intstack->nelems > 0) {
        printf("%d\n", int_stack_pop(intstack));
    }

    // push one more to check to see if our wiring
    // works when we removed all of the elements 
    int_stack_push(intstack, 7);
    printf("%d\n\n", int_stack_pop(intstack));

    // clean up
    free(intstack);
    
    return 0;
}

