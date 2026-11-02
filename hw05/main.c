#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "lock.h"

static int read_file(FILE *in);
/*
Takes FILE* as a text file input and parses codes line-by line. If pointer is null, return. 
Does not check for time based inputs, as inputs are a text file. 
*/

static int read_input(FILE *in);
/*
Takes FILE* as a standardinput and parses codes char-by-char. If pointer is null, return. 
Makes use of time-based checking for E_TIMEOUT event. 
*/

int main (int argc, char *argv[]) {
    /*
    main() takes input on launch and redirects to two separate input expectations.
    argc == 2 takes input as a text file and moves to read_file()
    argc <= 1 takes user input and moves to read_input()
     */
    FILE *input = stdin;
    int num_skips;

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [input-file]\n", argv[0]);
        return EXIT_FAILURE; //If too many opening arguments were supplied, exit 
    }

    if (argc == 2) {
        input = fopen(argv[1], "r"); //open text file
        if (input == NULL) { //if .txt file is NULL, exit. 
            perror(argv[1]);
            return EXIT_FAILURE;
        }
        num_skips = read_file(input);
    }
    else {
        num_skips = read_input(input); //read user input
    }

    printf("\nNumber of skips: %d\n", num_skips);

    return 0;
}


static int read_file(FILE *in){
    /*
    Takes FILE* as a text file input and parses codes line-by-line, char-by-char. 
    If pointer is null, return. Simulates timeout with chr = 't'. 

    Returns number of inputs skipped
    */
    state_t curr_state = LOCKED;
    int chr;
    int skipped = 0;
    int line = 1;
    int line_header_printed = 0;
    
    while ((chr = fgetc(in)) != EOF) {
        /* Check for newlines and null characters
        print header line if at the start of a new input.
        */ 
        if (chr == '\n' || chr == '\0') {
            if (!line_header_printed)
                printf("\n\nLINE %d\n", line);
            line++;
            line_header_printed = 0;
            curr_state = LOCKED;
            continue;
        }
        if (isspace((unsigned char)chr)) {
            continue; //parse past spaces. Should not be included in skipped inputs
        }

        //Avoid duplicate header lines
        if (!line_header_printed) {
            printf("LINE %d\n", line);
            line_header_printed = 1;
        }

        state_t prev_state = curr_state; //load prev state and open an action
        action_t action = A_NONE;

        int event = lock_event(curr_state, (char)chr);
        if (event < 0) { //check for invalid lock_event return
            skipped++;
            printf("  SKIPPED CHAR %c\n", (char)chr);
            continue;
        }

        curr_state = lock_step(curr_state, event, &action);
        if (curr_state < 0) { //check for invalid lock_step return
            curr_state = prev_state;
            skipped++;
            printf("  SKIPPED CHAR %c\n", (char)chr);
            continue;
        }

        //Grab names for printing
        const char* ps_name = state_name(prev_state);
        const char* s_name = state_name(curr_state);
        const char* e_name = event_name(event);
        const char* a_name = action_name(action);

        //print input for given line
        printf("  %s %c %s -> %s %s\n", ps_name, (char)chr, e_name, s_name, a_name);
    }

    return skipped;
}

static int read_input(FILE *in)
{
    /*
    Takes FILE* as a standardinput and parses codes char-by-char. If pointer is null, return. 
    Makes use of time-based checking for E_TIMEOUT event. 
    */
    state_t curr_state = LOCKED;
    int chr;
    int skipped = 0;
    time_t first_time = time(NULL);

    while ((chr = fgetc(in)) != 'X' && chr != 'x') {
        /* Exit when user types 'x' or 'X'
        */ 
        time_t now_time = time(NULL);
        if (now_time - first_time >= 5 && curr_state != OPEN && curr_state != LOCKED) {
            first_time = time(NULL); //Update time
            chr = 't';//Timeout procedure
        }
        else if (curr_state == LOCKED) 
            first_time = now_time;

        if (isspace((unsigned char)chr)) {
            continue; //parse past spaces. Should not be included in skipped inputs
        }

        state_t prev_state = curr_state;
        action_t action = A_NONE;

        int event = lock_event(curr_state, (char)chr);
        if (event < 0) { //check for invalid lock_event return
            skipped++;
            printf("\n  SKIPPED CHAR %c\n", (char)chr);
            continue;
        }
        curr_state = lock_step(curr_state, event, &action);
        if (curr_state < 0) { //check for invalid lock_step return
            curr_state = prev_state;
            skipped++;
            printf("\n  SKIPPED CHAR %c\n", (char)chr);
            continue;
        }

        //Grab names for printing
        const char* ps_name = state_name(prev_state);
        const char* s_name = state_name(curr_state);
        const char* e_name = event_name(event);
        const char* a_name = action_name(action);

        //print input for given line
        printf("\n  %s %c %s -> %s %s\n", ps_name, (char)chr, e_name, s_name, a_name);
    }

    return skipped;
}
