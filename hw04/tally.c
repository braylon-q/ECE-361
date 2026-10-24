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

void tally_destroy(tally_t **t)
{
    /*
    Empties a tally and makes it available for tally_create() to use again.
    A NULL input does nothing.
    */

    
    if (t == NULL)
        return;
    else if (*t == NULL) 
        return; // Do nothing if the input tally is NULL
    

    (*t)->size = -1; // Open tally for re-allocation.
    for (int i = 0; i < TALLY_NAMES; i++) {
        (*t)->names[i][0] = '\0'; // Clear each name in the tally
        (*t)->counts[i] = 0; // Reset the count for each name
    }

    *t = NULL; //Un-point t from the memory address inside of static array tallies[]
    //This means tally_create will have to look for empty tallies that also haven't been allocated yet.
}

int tally_add(tally_t *t, const char *name)
{
    /*
    Counts occurence of name and adds to to proper count index. 
    If t or name is NULL, if name is new and t->size >= TALLY_NAMES-1, or 
    if name exceeds TALLY_LEN, return -1. Else, return new count.
    */
   if (t == NULL || name == NULL) {
        return -1; // Return -1 if the tally or name is NULL
    }

    if (strlen(name) > TALLY_LEN || strlen(name) < 1) {
        return -1; // Return -1 if the name exceeds the maximum length or is empty
    }

    // Check if the name already exists in the tally
    for (int i = 0; i < t->size; i++) {
        if (strcmp(t->names[i], name) == 0) {
            t->counts[i]++; // Increment the count for the existing name
            return t->counts[i]; // Return the new count
        }
    }

    // If the name is new, check if there is space to add it
    if (t->size >= TALLY_NAMES) {
        return -1; // Return -1 if there is no space to add a new name
    }

    // Add the new name to the tally
    strncpy(t->names[t->size], name, TALLY_LEN); // Copy the name into the names array up to TALLY_LEN characters
    t->names[t->size][TALLY_LEN] = '\0'; // Ensure null-termination (protect against long names)
    t->counts[t->size] = 1; // Initialize the count for the new name
    t->size++; // Increment the size of the tally

    return 1; // Return 1 as this is the first occurrence of the new name
}

int tally_count(const tally_t *t, const char *name)
{
    /*
    Returns how many times name has been added to tally *t.
    If t or name is NULL, or if name is not in the tally, return -1.
    Callers can see list of names by using tally_print().
    */
    if (t == NULL || name == NULL) {
        return -1; // Return -1 if the tally or name is NULL
    }

    // Check if the name exists in the tally
    for (int i = 0; i < t->size; i++) {
        if (strcmp(t->names[i], name) == 0) {
            return t->counts[i]; // Return the count for the existing name
        }
    }

    return -1; // Return -1 if the name is not in the tally
}

int tally_size(const tally_t *t)
{
    /*
    Returns how many different names are in tally *t.
    If t is NULL, return -1.
    */
    if (t == NULL) {
        return -1; // Return -1 if the tally is NULL
    }

    return t->size; // Return the current size of the tally
}

void tally_print(const tally_t *t, FILE *out)
{
    /*
    Prints the names and counts in tally *t to the given output stream.
    Must be printed in alphabeitical order, one name and count per line, in the format "name: count".
    Note: tried to do recursively, but it would've required too much complexity.
    If t or out is NULL, do nothing.
    */
    if (t == NULL || out == NULL) {
        return;
    }

    int arr[TALLY_NAMES] = {0}; // Number of names lexically greater than each name.
    for (int i = 0; i < t->size; i++) { //Loop through all possible name comparisons.
        for (int j = 0; j < t->size; j++) {
            if (strcmp(t->names[i], t->names[j]) < 0) {
                arr[i]++; // Increment the count for names that are greater than the current name
            }
        }
    }
    for (int i = t->size-1; i >= 0; i--) { //Check for highest count (lowest alphabetical name) and print it, then check for next highest count, etc.
        for (int j = 0; j < t->size; j++) {
            if (arr[j] == i) {
                fprintf(out, "%s: %d\n", t->names[j], t->counts[j]); // Print the name and count in alphabetical order
            }
        }
    }
}


