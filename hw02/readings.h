#pragma once

// #define LINE_LEN     128 KEEPING THESE IN MAIN FOR TEST EDITING IF NEEDED
// #define MAX_READINGS 1000

typedef struct {
    int count;
    int skipped;
} readingStats;

/*
currently just 1 function within readings.h. Parses text file and fills arrays,
though does not return them, as arrays are passed using pointers. Instead, # readings
and # skipped lines returned.
*/

readingStats readings_parse(FILE* stream_in,int* ticks_r, float* temps_r, float* hums_r, int max_count, const int line_length, const int max_readings);
/*
Takes stdin from main.c, and pointers to our three arrays (ticks, temps, hums) 
to fill them with parsed data. Returns the number of readings successfully parsed,
and number of those skipped.
*/

