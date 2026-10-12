/*
 * readings.c: ECE 361, Fall 2026, HW2 starter
 *
 * Reads sensor readings from standard input and prints a summary: the number
 * of readings, the minimum, maximum, and mean temperature and humidity, the
 * longest run of readings above a temperature threshold, and a histogram of
 * the temperatures.
 *
 * Input: one reading per line, "tick temperature humidity", for example
 *     12  23.5  41.0
 * Blank lines and lines starting with '#' are ignored. A line that does not
 * hold three numbers is counted as skipped.
 *
 * Usage: ./readings [threshold] < data/normal.txt
 *        threshold is in degrees C, default 30.0
 *
 * The program works. It is also one file, with global variables, repeated
 * code, and a main() that does everything. HW2 asks you to split it into
 * modules without changing what it prints.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readings.h"
#include "stats.h"
#include "histogram.h"

#define MAX_READINGS 1000
#define LINE_LEN     128
#define NUM_BINS     20          /* histogram: 0 to 100 C in 5 C bins */
#define BIN_WIDTH    5.0f

int main(int argc, char *argv[]) {
    //No global variables; everything is local to main() or passed as arguments.
    int   ticks[MAX_READINGS];
    float temps[MAX_READINGS];
    float hums[MAX_READINGS];

    //Threshold value for temperature readings. Change this to adjust the threshold.
    float threshold = 30.0f;

    //ARGc/argv checks stay in main.c
    if (argc > 2) {
        fprintf(stderr, "usage: %s [threshold] < readings.txt\n", argv[0]);
        return 1;
    }
    if (argc == 2) {
        char *end;
        threshold = strtof(argv[1], &end);
        if (*end != '\0') {
            fprintf(stderr, "error: threshold '%s' is not a number\n", argv[1]);
            return 1;
        }
    }

    //gives count of successful and skipped readings. Fills ticks, temps and hums from stdin.
    readingStats reading_main = readings_parse(stdin, ticks, temps, hums, LINE_LEN, MAX_READINGS);

    if (reading_main.count == 0) {
        return 0;
    }

    /* temperature: min, max, and mean */
    stats temp_stats = stats_fill(temps, reading_main.count); //fill temperature stats to structure

    //printf call stays in main
    printf("temperature: min %6.1f  max %6.1f  mean %6.2f C\n", temp_stats.min, temp_stats.max, temp_stats.mean);

    /* humidity: min, max, and mean */
    stats hum_stats = stats_fill(hums, reading_main.count); //fill humidity stats to structure

    //printf call stays in main
    printf("humidity:    min %6.1f  max %6.1f  mean %6.2f %%RH\n", hum_stats.min, hum_stats.max, hum_stats.mean);

    //gives the longest run of consecutive readings above the threshold.
    bestRun main_best = stats_threshold(temps, reading_main.count, threshold);

    //These if-else statements stay in main to avoid passing the ticks array through to 
    //stats 
    if (main_best.best == 0)
        printf("above %.1f C: never\n", threshold);
    else
        printf("above %.1f C: longest run %d readings, from tick %d to tick %d\n",
               threshold, main_best.best, ticks[main_best.best_start], ticks[main_best.best_start + main_best.best - 1]);

    //print histogram of temperatures
    histogram_print(temps, reading_main.count, NUM_BINS, BIN_WIDTH);

    return 0;
}
