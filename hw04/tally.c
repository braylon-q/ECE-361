#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tally.h"

// Define the structure for tally
typedef struct tally {
    char names[TALLY_NAMES][TALLY_LEN + 1]; // A 2D Array to hold names, with each row holding one name and each column one character
    int counts[TALLY_NAMES]; // Array to hold counts for each name
    int size; // Current number of names in the tally
} tally_t;

tally_t *tally_create(void)
{
    /*
    Create a new empty tally and return a pointer to it. 
    If the tally cannot be created (too many tally structures), return NULL.
    Not using malloc(), as it is not allowed in this project. 
    */
    static int created_count = 0;
    static int initialized = 0;
    static tally_t tallies[TALLY_MAX];

    if (!initialized) {
        for (int i = 0; i < TALLY_MAX; i++) {
            tallies[i].size = -1; //Fill all size fields with -1 on first run only to indicate they are free for allocating.
        }
        initialized = 1;
    }

    for (int i = 0; i < TALLY_MAX; i++) {
        if (tallies[i].size == -1) { // Check for an unallocated tally
            tally_t *new_tally = &tallies[i];
            new_tally->size = 0; // Set size to 0 to indicate allocation
            return new_tally; // Return the pointer to the newly created tally
        }
    }
    
    return NULL; // No avaialble spaces for new tallies.
}

void tally_destroy(tally_t *t)
{
    /*
    Empties a tally and makes it available for tally_create() to use again.
    A NULL input does nothing.
    */

    if (t == NULL) {
        return; // Do nothing if the input tally is NULL
    }

    t->size = -1; // Open tally for re-allocation.
    for (int i = 0; i < TALLY_NAMES; i++) {
        t->names[i][0] = '\0'; // Clear each name in the tally
        t->counts[i] = 0; // Reset the count for each name
    }
    //This means tally_create will have to look for empty tallies that also haven't been allocated yet.
}

int tally_add(tally_t *t, const char *name)
{
    /*
    Counts occurence of name and adds to to proper count index. 
    If t or name is NULL, if name is new and t->size equals/exceeds TALLY_NAMES-1, or 
    if name exceeds TALLY_LEN, return -1. Else, return new count.
    */

    
}