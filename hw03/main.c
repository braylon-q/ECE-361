#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ptr.h"

static void print_array(const float *arr, int n) {
    /*
    Helper function to print the contents of an array of floats.
    */
    if (arr == NULL || n <= 0) {
        printf("Array is empty or NULL.\n");
        return;
    }
    
    for (int i = 0; i < n; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");
}


int main (void) {
    /*
    MANUAL TESTING: Non-extensive testing of functions in ptr.c.
    All outputs will print to terminal for manual verification. 
    May require helper function.
    */
    //TESTING swap_float
    float x = 5.0f;
    float y = 10.0f;
    printf("Before swap: x = %.2f, y = %.2f\n", x, y);
    swap_float(&x, &y);
    printf("After swap: x = %.2f, y = %.2f\n", x, y);

    printf("\n");
    //TESTING minmax
    float arr[] = {3.5f, 2.1f, 5.7f, 1.2f, 4.8f, 6.4f, 9.1f, 1.4f, 1.21f, 8.7f};
    int n = sizeof(arr) / sizeof(float);
    float min_main, max_main;

    if (minmax(arr, n, &min_main, &max_main) == 0) {
        printf("Minimum: %.2f, Maximum: %.2f\n", min_main, max_main);
    } else {
        printf("Error occurred while finding min/max.\n");
    }

    printf("\n");
    //TESTING findAbove (using same array as minmax)
    float threshold_main = 4.0f;
    const float *result_main = findAbove(arr, n, threshold_main);
    if (result_main != NULL) {
        printf("First value above %.2f: %.2f\n", threshold_main, *result_main);
    } else {
        printf("No value found above %.2f.\n", threshold_main);
    }

    printf("\n");
    //TESTING reverse
    printf("Array before reverse: ");
    print_array(arr, n);
    reverse(arr, n);
    printf("Array after reverse: ");
    print_array(arr, n);

    return 0;
}