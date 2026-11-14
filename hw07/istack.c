#include <stdio.h>
#include <stdlib.h>
#include "istack.h"
#include <stdbool.h>

typedef struct istack {
    int *data;
    int count;
    int cap;
} istack_t;

istack_t *istack_create(void) {
    /*
    Allocates a new stack *s as well as its data array, where the data array is 
    of ISTACK_START size. Initialize the count to 0 and cap to ISTACK_START. 
    Frees s if the data array allocation fails.

    Returns NULL if either allocation fails.
    */

    //Allocate s and check for no space
    istack_t *s = malloc(sizeof(istack_t));
    if (s == NULL) {
        return NULL;
    }

    //allocate data array pointer. If NULL free(s) and return NULL.
    s->data = malloc(ISTACK_START * sizeof(int));
    if (s->data == NULL) {
        free(s);
        return NULL;
    }

    //Initialize rest of structure.
    s->count = 0;
    s->cap = ISTACK_START;
    return s;
}

void istack_destroy(istack_t *s) {
    /*
    Frees the data array and then the stack of input sto prevent memory leaks. Returns
    nothing.
    */
    if (s == NULL) return;
    free(s->data);
    free(s);
}

int istack_push(istack_t *s, int v){
    /*
    Parameters: istack_t *s, integer (to be pushed) v.
    Pushes int v to stack if at data + count if there is capacity.
    Otherwise, doubles the size of data and then pushes v. Increments count after.
    Returns 0 on success or -1 if stack cannot grow, or if s is NULL.
    */
    if (s == NULL) {
        return -1;
    }

    if (s->count == s->cap) {
        //Creates temp variables for cap and data in case of
        //allocation failure to preserve previous stack.
        int new_cap = s->cap * 2;
        int *new_data = realloc(s->data, new_cap * sizeof(int));
        if (new_data == NULL) {
            return -1;
        }
        //update stack with new values.
        s->data = new_data;
        s->cap = new_cap;
    }

    s->data[s->count] = v;
    s->count++; //increment count after pushing v
    return 0;
}

int istack_pop(istack_t *s, int *v){
    /*
    Parameters: istack_t *s, pointer to integer (to be popped) v.
    Place the top value in the stack s into *v and decrement count, "freeing" v. 
    Returns 0 on success or -1 if the stack is empty, or if s or v is NULL.
    */

    if (s == NULL || v == NULL || s->count == 0) {
        return -1;
    }

    s->count--; //decrement count beforehand to get top value.
    *v = s->data[s->count];
    return 0;
}

int istack_peek(const istack_t *s, int *v){
    /*
    Parameters: istack_t *s, pointer to integer (to be peeked) v.
    Place the top value in the stack s into *v and decrement count, leaving v in stack. 
    Returns 0 on success or -1 if the stack is empty, or if s or v is NULL.
    */
    
    if (s == NULL || v == NULL || s->count == 0) {
        return -1;
    }

    *v = s->data[s->count - 1];
    return 0;
}

size_t istack_count(const istack_t *s){
    /*
    Parameters: istack_t *s.
    Returns the number of elements in stack s, or 0 if s is NULL.
    */
    if (s == NULL) {
        return 0;
    }
    return s->count;
}

bool istack_is_empty(const istack_t *s){
    /*
    Parameters: istack_t *s.
    Returns true if stack s is empty or s is NULL, false otherwise.
    */
    if (s == NULL) {
        return true;
    }
    return s->count == 0;
}

size_t istack_capacity(const istack_t *s){
    /*
    Parameters: istack_t *s.
    Returns the capacity of stack s, or 0 if s is NULL.
    */
    if (s == NULL) {
        return 0;
    }
    return s->cap;
}

int *istack_to_array(const istack_t *s, size_t *n){
    /*
    Parameters: constant istack_t *s, pointer to size_t n.
    Create a new array of values, bottom first, and their number in *n (given by *s).
    Dynamically allocated for as much memory as needed. No freeing done by function.
    If any parameter is NULL, set *n to 0 and return NULL.
    */
    if (s == NULL || n == NULL) {
        if (n != NULL) {
            *n = 0;
        }
        return NULL;
    }
    
    //Get size of new array
    *n = s->count;
    //allocate new array and check for allocation failure
    int *array = malloc(*n * sizeof(int));
    if (array == NULL) {
        *n = 0;
        return NULL;
    }
    
    //copy values from stack to array
    for (size_t i = 0; i < *n; i++) {
        array[i] = s->data[i];
    }
    return array; //return address of new array in heap.
}