#pragma once

void swap_float(float* a, float* b);
/*
Takes two pointers, a and b, and swaps the values they point to. 
This is done in 3 operations, using one temporary variable.
Returns nothing. If either pointer is NULL, does nothing.
*/

int minmax(const float *a, int n, float *min, float *max);
/*
Takes an array of floats a of size n, and finds the minimum and maximum values within 
said array. Also returns 0 if any pointer comes in NULL. The minimum and maximum values are stored 
in the variables pointed to by min and max, respectively. 
Returns 0 if successful, and -1 if array is empty (n <= 0) or any pointer is NULL.
*/

const float *findAbove(const float *a, int n, float threshold);
/*
Takes an array of floats a of size n, and finds the first value in the array that is
greater than the threshold value. Returns a pointer to that value, or NULL if no such
value exists. If the array is empty (n <= 0), or if the pointer a is NULL, returns NULL.
*/

void reverse(float *a, int n);
/*
Takes an array of floats a of size n, and reverses the order of elements in the array.
Uses temporary pointers for arithmetic operations along with for loops. 
If the array is empty (n <= 0), or if the pointer a is NULL, does nothing.
*/

