#include <stdio.h>
#include <stddef.h>
#include "istack.h"

int main(void) {
    /*
     * Read stdin character by character, tracking bracket characters on a
     * stack. Brackets inside strings and comments count; the first mismatch
     * stops parsing. Report pair count and maximum depth on success.
     */
    istack_t *stack = istack_create();
    if (stack == NULL) {
        printf("error: unable to create stack\n");
        return 1;
    }

    size_t pairs = 0;
    size_t deepest = 0;
    size_t line = 1;
    size_t column = 1;
    int ch;

    while ((ch = getchar()) != EOF) {
        if (ch == '(' || ch == '[' || ch == '{') {
            if (istack_push(stack, ch) != 0) {
                printf("error: unable to store opening bracket\n");
                istack_destroy(stack);
                return 1;
            }
            size_t depth = istack_count(stack);
            if (depth > deepest) {
                deepest = depth;
            }
        } else if (ch == ')' || ch == ']' || ch == '}') {
            int open;
            if (istack_peek(stack, &open) != 0) {
                printf("line %zu, column %zu: found '%c', nothing open\n",
                       line, column, ch);
                istack_destroy(stack);
                return 1;
            }

            int expected = open == '(' ? ')' : open == '[' ? ']' : '}';
            if (ch != expected) {
                printf("line %zu, column %zu: found '%c', expected '%c'\n",
                       line, column, ch, expected);
                istack_destroy(stack);
                return 1;
            }

            (void)istack_pop(stack, &open);
            pairs++;
        }

        if (ch == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }

    if (!istack_is_empty(stack)) {
        int innermost;
        (void)istack_peek(stack, &innermost);
        printf("end of input: %zu still open, innermost '%c'\n",
               istack_count(stack), innermost);
        istack_destroy(stack);
        return 1;
    }

    printf("balanced: %zu pairs, deepest %zu\n", pairs, deepest);
    istack_destroy(stack);
    return 0;
}