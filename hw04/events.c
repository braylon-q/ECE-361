#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "tally.h"
#define MAX_FILE_LINES 1000


int main() {
    /*
    This program reads standard input line by line with fgets(), removes
    the newline, skips blank lines and lines that start with #, and adds every other line
    to a single tally. Afterword, it does a print: tally_print() followed by Skips: S, where
    S is strictly the lines skipped by tally_add itself.
    */
    tally_t *event = tally_create();
    char line[1024]; // Leave room for input lines longer than a tally name.
    uint32_t S = 0, P = 0;

    if (event == NULL) {
        fprintf(stderr, "Could not create tally\n");
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof line, stdin) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';

        int i = 0;
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (line[i] == '\0' || line[i] == '#')
            continue;

        size_t name_len = strlen(line + i); //get length of characters + i memory widths after leading whitespace read
        while (name_len > 0 && 
               (line[i + name_len - 1] == ' ' || line[i + name_len - 1] == '\t'))
            line[i + --name_len] = '\0'; //pad end of reading with NULL chars

        if ((S+P) >= MAX_FILE_LINES)
            break;
        if (tally_add(event, line + i) == -1)
            S++;
        else
            P++;
    }
    if (tally_add(NULL, NULL) == -1) S++; //deliberately skip a line for testing.
    tally_print(event, stdout);
    printf("Skips: %d\n", S);
    tally_destroy(&event);
}
