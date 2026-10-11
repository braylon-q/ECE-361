#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"

static void print_thresh(bestRun* run);
/*
Helper function to print stats_threshold outcome.
*/

stats stats_fill(float* array, int count_s)
{
    /*
    Runs a for loop to calculate the mean, min, and max of the given array.
    */
    stats result;
    //fill result with baseline values before moving to for loop.
    result.mean = 0.0f;
    result.min = array[0];
    result.max = array[0];

    for (int i = 0; i < count_s; i++) {
        if (array[i] < result.min)
            result.min = array[i];
        if (array[i] > result.max)
            result.max = array[i];
        result.mean += array[i];
    }
    result.mean /= count_s; //divide total by count to get mean

    return result; //printing will be done in main() as different arrays will need
                   //different printing outputs.

}

bestRun stats_threshold(float* array, int count_s, float threshold)
{
    /*
    Runs a for loop to find the best run of values above the given threshold.
    */
    bestRun result;
    //fill array values before starting the loop.
    result.best = 1;
    result.best_start = -1;
    int run = 0;
    int start = 0;

    for (int i = 0; i < count_s; i++) {
        if (array[i] > threshold) {
            if (run == 0)
                start = i;
            run++;
            if (run > result.best) {
                result.best = run;
                result.best_start = start;
            }
        } else {
            run = 0;
        }
    }
    
    return result;
}

//FIXME: will instead handle printing inside of main() to avoid having to pass through
//the ticks array.

// static void print_thresh(bestRun* run, float )
// {
//     /*
//     Helper function to print stats_threshold outcome.
//     */
//     if (best == 0)
//         printf("above %.1f C: never\n", threshold);
//     else
//         printf("above %.1f C: longest run %d readings, from tick %d to tick %d\n",
//                threshold, best, ticks[best_start], ticks[best_start + best - 1]);
// }