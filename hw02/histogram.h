#pragma once

/*
single void function to print histogram. Returns nothing.
*/

void histogram_print(int *temps_h, int count_h, const int num_bins, const float bin_width);
/*
Prints histogram through 2 loops; First loop fills an array of bins,
second loop prints the histogram to txt file. 
*/