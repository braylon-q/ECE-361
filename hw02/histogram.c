#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "histogram.h"

void histogram_print(int *temps_h, int count_h, const int num_bins, const float bin_width)
{
    /*
    Prints histogram through 2 loops; First loop fills an array of bins,
    second loop prints the histogram to txt file. 
    */
    
    /* histogram of temperatures, 5 C bins; values outside 0..100 go to the end bins */
    int bins[num_bins]; // initialize bins
    memset(bins, 0, sizeof(bins)); // fill all bins with 0's

    for (int i = 0; i < num_bins; i++)
        bins[i] = 0; // initialize all counts to zero
    for (int i = 0; i < count_h; i++) {
        int b = (int) (temps_h[i] / bin_width); //figure out which bin to place bins[i] in
        if (temps_h[i] < 0.0f)
            b = 0; //b = 0 if temps[i] at min edge case (0 Celsius)
        if (b >= num_bins)
            b = num_bins - 1; //b = maximum index if temps[i] at max edge case (100 Celsius)
        bins[b]++; //increment the count for the appropriate bin
    }
    printf("histogram:\n");
    for (int b = 0; b < num_bins; b++) {
        if (bins[b] == 0)
            continue; //skip empty bins
        printf("  %5.1f to %5.1f C | ", b * bin_width, (b + 1) * bin_width ); //print the range for the current bin
        for (int k = 0; k < bins[b]; k++)
            putchar('*'); //print a star for each reading in the current bin
        printf(" %d\n", bins[b]); //print the count for the current bin
    }   


}