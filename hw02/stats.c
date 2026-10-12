#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"

static float stats_max(float* array, int count_s, int i);

stats stats_fill(float* array, int count_s)
{
    /*
    Runs a for loop to calculate the mean, min, and max of the given array.
    */
    stats result;
    //fill result with baseline values before moving to for loop.
    result.mean = 0.0f;
    result.min = array[0];
    result.max = stats_max(array, count_s, 0);

    for (int i = 0; i < count_s; i++) {
        if (array[i] < result.min)
            result.min = array[i];
        // if (array[i] > result.max)
        //     result.max = array[i];
        result.mean += array[i];
    }
    result.mean /= count_s; //divide total by count to get mean

    return result; //printing will be done in main() as different arrays will need
                   //different printing outputs.

}

//helper function: stats_max, replaces this function thats already within 
//stats_fill just to statisfy assignment requirement.
static float stats_max(float* array, int count_s, int i)
{
    /*
    Recursive function which finds maximum value of an array starting from index i.
    Calls itself recursively until it reaches the last element of the array, then,
    compares itself with the previous array value, returning the greater of the two.
    Closes frames until the original call is resolved. Recursive maximum = 9999.
    */
    if (i == count_s - 1 || i >= 9998) //stop recursion at last element or 
        return array[i];               // defined recursion limit.
    
    float rest = stats_max(array, count_s, i + 1);
    return array[i] > rest ? array[i] : rest; //layout copied from starter program
}

bestRun stats_threshold(float* array, int count_s, float threshold)
{
    /*
    Runs a for loop to find the best run of values above the given threshold.
    */
    bestRun result;
    //fill array values before starting the loop.
    result.best = 0;
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
