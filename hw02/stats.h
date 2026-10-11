#pragma once

//structure used for data uniformity and returning information to
//main.c without passing many references.
typedef struct {
    float mean;
    float min;
    float max;
} stats;

typedef struct {
    int best;
    int best_start;
} bestRun;

/*
2 functions: one to deal with filling all mean, min, and max values
for passed array. Other to deal with threshold run checking.

int run = 0, curr_best = 0, curr_best_start = -1, start = 0;
*/

stats stats_fill(float* array, int count_s);
/*
Runs a for loop to calculate the mean, min, and max of the given array.
*/

bestRun stats_threshold(float* array, int count_s, float threshold);
/*
Runs a for loop to find the best run of values above the given threshold.
*/


