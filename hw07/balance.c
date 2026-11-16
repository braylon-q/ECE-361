#include <stdio.h>
#include <stddef.h>
#include "istack.h"

int main(void) {
    /*
     * Read stdin character by character, tracking bracket characters on a
     * stack. Brackets inside strings and comments count; the first mismatch
     * stops parsing. Report pair count and maximum depth on success.
     */
    
    //create and check for stack allocation
    istack_t *stack = istack_create();
    if (stack == NULL) {
        printf("error: unable to create stack\n");
        return 1;
    }

    //Set up tracking variables 
    size_t pairs = 0;
    size_t deepest = 0;
    size_t line = 1;
    size_t column = 1;
    int ch;

    //Check for empty input file. If empty, print failure message and exit with return code 1.
    if ((ch = getchar()) == EOF) {
        printf("error: empty input file");
        printf("     exit status 1\n");
        istack_destroy(stack);
        return 1;
    } else { //return the character to stdin for processing 
        ungetc(ch, stdin);
    }



    while ((ch = getchar()) != EOF) {
        //Check for opening brackes | make sure they are pushed
        if (ch == '(' || ch == '[' || ch == '{') {
            if (istack_push(stack, ch) != 0) {
                printf("error: unable to store opening bracket");
                printf("     exit status 1\n");
                istack_destroy(stack);
                return 1;
            }
            size_t depth = istack_count(stack);
            if (depth > deepest) {
                deepest = depth;
            }
        //Check for closing brackets
        } else if (ch == ')' || ch == ']' || ch == '}') {
            int open;
            //Check to see if anything is on the stack. Store open bracket in var "open"
            if (istack_peek(stack, &open) != 0) {
                printf("line %zu, column %zu: found '%c', nothing open",
                       line, column, ch);
                printf("     exit status 1\n");
                istack_destroy(stack);
                return 1;
            }
            //Assign expected closing bracket. Check for matching bracket
            //NOTE: Default case of '}' should work: a previous closed bracket being on the stack
            //would have already been popped.
            int expected = open == '(' ? ')' : open == '[' ? ']' : '}';
            if (ch != expected) {
                printf("line %zu, column %zu: found '%c', expected '%c'",
                       line, column, ch, expected);
                printf("     exit status 1\n");
                istack_destroy(stack);
                return 1;
            }
            //pop open bracket (they're the only ones pushed to the stack)
            (void)istack_pop(stack, &open);
            pairs++;
        }

        //Check for newlines and adjust column variable (used in errors)
        if (ch == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }

    //Check for stacks that have not been fully popped during character checking
    if (!istack_is_empty(stack)) {
        int innermost;
        (void)istack_peek(stack, &innermost);
        printf("end of input: %zu still open, innermost '%c'",
               istack_count(stack), innermost);
        printf("     exit status 1\n");
        istack_destroy(stack);
        return 1;
    }

    //Successful output printing.
    printf("balanced: %zu pairs, deepest %zu", pairs, deepest);
    printf("     exit status 0\n");
    istack_destroy(stack);


    return 0;
}