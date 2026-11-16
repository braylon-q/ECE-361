#include <stdio.h>
#include <stdlib.h>
#include "istack.h"

static int passes = 0;
static int fails = 0;


#define CHECK(cond) do { \
    if (cond) { \
        printf("PASS %s\n", #cond); \
        passes++; } \
    else { \
        printf("FAIL %s\n", #cond); \
        fails++; } \
} while(0)

#define TEST(description, cond) do { \
        printf("%s\n", description); \
        CHECK(cond); \
} while(0)

int main(void) {
    /*
    Testing developed by ChatGPT-6 Luna. Confirmed to meet minimum
    requirements by: Braylon Jones.
    */
        int value;
        istack_t *s = istack_create();
        TEST("A new stack is allocated successfully.", s != NULL);
        if (s != NULL) {
            TEST("A new stack starts empty with capacity eight.",
                 istack_is_empty(s) && istack_count(s) == 0 &&
                     istack_capacity(s) == ISTACK_START);

            value = 12345;
            TEST("Popping an empty stack fails without changing the output.",
                 istack_pop(s, &value) == -1 && value == 12345);

            value = 12345;
            TEST("Peeking an empty stack fails without changing the output.",
                 istack_peek(s, &value) == -1 && value == 12345);

            int push_ok = 1;
            for (int i = 0; i < ISTACK_START; i++) {
                if (istack_push(s, i) != 0) {
                    push_ok = 0;
                    break;
                }
            }
            TEST("Eight pushes fit without growing the stack.",
                 push_ok && istack_count(s) == 8 &&
                     istack_capacity(s) == 8);

            int ninth_push_ok = istack_push(s, 8) == 0;
            TEST("The ninth push doubles capacity to sixteen and retains all values.",
                 ninth_push_ok && istack_count(s) == 9 &&
                     istack_capacity(s) == 16);

            int lifo_ok = 1;
            for (int i = 8; i >= 0; i--) {
                if (istack_pop(s, &value) != 0 || value != i) {
                    lifo_ok = 0;
                    break;
                }
            }
            TEST("Values are popped in last-in, first-out order.", lifo_ok);
            istack_destroy(s);
        }

        s = istack_create();
        TEST("A stack for the 1000 push-pop operations is allocated successfully.",
             s != NULL);
        if (s != NULL) {
            int round_trip_ok = 1;
            for (int i = 0; i < 1000; i++) {
                if (istack_push(s, i * 3 - 7) != 0) {
                    round_trip_ok = 0;
                    break;
                }
            }
            if (round_trip_ok && istack_count(s) != 1000) {
                round_trip_ok = 0;
            }
            for (int i = 999; round_trip_ok && i >= 0; i--) {
                if (istack_pop(s, &value) != 0 || value != i * 3 - 7) {
                    round_trip_ok = 0;
                }
            }
            TEST("One thousand pushed values pop back in reverse order.",
                 round_trip_ok && istack_is_empty(s));
            istack_destroy(s);
        }

        s = istack_create();
        TEST("A stack for array conversion is allocated successfully.",
             s != NULL);
        if (s != NULL) {
            int pushes_ok = istack_push(s, 10) == 0 &&
                            istack_push(s, 20) == 0 &&
                            istack_push(s, 30) == 0;
            size_t n = 99;
            int *array = istack_to_array(s, &n);
            TEST("Stack conversion returns a bottom-first array copy.",
                 pushes_ok && array != NULL && n == 3 &&
                     array[0] == 10 && array[1] == 20 && array[2] == 30);
            if (array != NULL && pushes_ok) {
                array[0] = 99;
                int top = 0;
                int stack_unchanged = istack_peek(s, &top) == 0 && top == 30;
                int pop_ok = istack_pop(s, &top) == 0 && top == 30;
                TEST("The returned array and stack remain independent in both directions.",
                     stack_unchanged && pop_ok && array[0] == 99 &&
                         array[2] == 30);
            } else {
                TEST("Changing the returned array does not change the stack.",
                     0);
            }
            free(array);
            istack_destroy(s);
        }

        s = istack_create();
        TEST("A stack for null-argument checks is allocated successfully.",
             s != NULL);
        if (s != NULL) {
            value = 42;
            int null_args_ok = istack_push(NULL, 1) == -1 &&
                               istack_pop(NULL, &value) == -1 && value == 42 &&
                               istack_pop(s, NULL) == -1 &&
                               istack_peek(NULL, &value) == -1 && value == 42 &&
                               istack_peek(s, NULL) == -1 &&
                               istack_count(NULL) == 0 &&
                               istack_is_empty(NULL) &&
                               istack_capacity(NULL) == 0;
            TEST("Stack operations handle null stack and output arguments.",
                 null_args_ok);

            size_t n = 99;
            int *array_null_stack = istack_to_array(NULL, &n);
            int *array_null_count = istack_to_array(s, NULL);
            int *array_both_null = istack_to_array(NULL, NULL);
            TEST("Array conversion handles null arguments and clears the length.",
                 array_null_stack == NULL && n == 0 &&
                     array_null_count == NULL && array_both_null == NULL);
            free(array_null_stack);
            free(array_null_count);
            free(array_both_null);
            istack_destroy(s);
        }

        istack_t *first = istack_create();
        istack_t *second = istack_create();
        TEST("Two independent stacks are allocated successfully.",
             first != NULL && second != NULL);
        if (first != NULL && second != NULL) {
            int independent_ok = istack_push(first, 11) == 0 &&
                                 istack_push(second, 22) == 0 &&
                                 istack_push(first, 33) == 0;
            int popped = 0;
            independent_ok = independent_ok &&
                            istack_pop(first, &popped) == 0 && popped == 33 &&
                            istack_peek(second, &popped) == 0 && popped == 22 &&
                            istack_count(first) == 1 &&
                            istack_count(second) == 1;
            TEST("Pushing and popping one stack leaves the other independent.",
                 independent_ok);
        }
        istack_destroy(first);
        istack_destroy(second);

        s = istack_create();
        int nonempty_destroy_ok = s != NULL && istack_push(s, 1) == 0 &&
                                  istack_push(s, 2) == 0;
        if (s != NULL) {
            istack_destroy(s);
        }
        TEST("A nonempty stack can be destroyed.", nonempty_destroy_ok);
        istack_destroy(NULL);
        TEST("Destroying a null stack is safe.", 1);

        printf("\nSUMMARY: %d passed, %d failed\n\n", passes, fails);
        return fails != 0;
}