#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ptr.h"

void swap_float(float* a, float* b)
{
    /*
    Takes two pointers, a and b, and swaps the values they point to. 
    This is done in 3 operations, using one temporary variable.
    Returns nothing. If either pointer is NULL, does nothing.
    */
    if (a == NULL || b == NULL) {
        return; // Do nothing if either pointer is NULL
    }
    
    float temp = *a; // Store the value pointed to by a in a temporary variable
    *a = *b;         // Assign the value pointed to by b to the location pointed to by a
    *b = temp;      // Assign the value stored in temp to the location pointed to by b
}

int minmax(const float *a, int n, float *min, float *max)
{
    /*
    Takes an array of floats a of size n, and finds the minimum and maximum values within 
    said array. Also returns 0 if any pointer comes in NULL. The minimum and maximum values are stored 
    in the variables pointed to by min and max, respectively. 
    Returns 0 if successful, and -1 if array is empty (n <= 0) or any pointer is NULL.
    */
    if (a == NULL || min == NULL || max == NULL) {
        return -1; // Return -1 if any pointer is NULL
    }
    
    if (n <= 0) {
        return -1; // Return -1 if the array is empty
    }
    
    *min = a[0]; // Initialize min to the first element
    *max = a[0]; // Initialize max to the first element
    
    for (int i = 1; i < n; i++) {
        if (a[i] < *min) {
            *min = a[i]; // Update min if a smaller value is found
        }
        if (a[i] > *max) {
            *max = a[i]; // Update max if a larger value is found
        }
    }
    
    return 0; // Return 0 to indicate success
}

const float *findAbove(const float *a, int n, float threshold)
{
    /*
    Takes an array of floats a of size n, and finds the first value in the array that is
    greater than the threshold value. Returns a pointer to that value, or NULL if no such
    value exists. If the array is empty (n <= 0), or if the pointer a is NULL, returns NULL.
    */
    if (a == NULL || n <= 0) {
        return NULL; // Return NULL if the array is empty or pointer is NULL
    }

    for (int i = 0; i < n; i++) {
        if (a[i] > threshold) {
            return &a[i]; // Return pointer to the first value greater than threshold
        }
    }

    return NULL; // Return NULL if no value is greater than the threshold
}

void reverse(float *a, int n)
{
    /*
    Takes an array of floats a of size n, and reverses the order of elements in the array.
    Uses temporary pointers for arithmetic operations along with for loops. 
    If the array is empty (n <= 0), or if the pointer a is NULL, does nothing.
    Must use the swap_float function to swap the elements.
    */
    if (a == NULL || n <= 0) {
        return; // Do nothing if the array is empty or pointer is NULL
    }

    for (float *low = a, *high = a + n - 1; low < high; low++, high--) {
        swap_float(low, high); // Swap the elements at low and high pointers
    }
}
