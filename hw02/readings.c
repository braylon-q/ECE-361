#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readings.h"

static void printer(readingStats stats);
/*
Prints parsing statistics (count and skipped).
Not externally accessible. Declared here for that reason.
*/


readingStats readings_parse(FILE* stream_in, int* ticks_r, float* temps_r, float* hums_r, int max_count, const int line_length, const int max_readings)
{
    /*
    Takes stdin from main.c, and pointers to our three arrays (ticks, temps, hums) 
    to fill them with parsed data. Returns the number of readings successfully parsed,
    and number of those skipped as a structure.  
    */
    readingStats parsed_Stats;
    char line[line_length];

    while (fgets(line, sizeof line, stream_in) != NULL) {
        int i = 0;
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (line[i] == '\n' || line[i] == '\0' || line[i] == '#')
            continue;
        if (parsed_Stats.count == max_readings) {
            fprintf(stderr, "warning: more than %d readings, the rest are ignored\n", max_readings);
            break;
        }
        if (sscanf(line, "%d %f %f", ticks_r[parsed_Stats.count], temps_r[parsed_Stats.count], hums_r[parsed_Stats.count]) == 3)
            parsed_Stats.count++;
        else
            parsed_Stats.skipped++;
    }

    printer(parsed_Stats);

    return parsed_Stats;
}

static void printer(readingStats stats)
{
    printf("Readings parsed: %d\n", stats.count);
    printf("Readings skipped: %d\n", stats.skipped);
    if (stats.count == 0) {
        printf("no readings, no summary\n");
    }
}

//SEEMS TO BE SET UP: wait for testing to change anything.