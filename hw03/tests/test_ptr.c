#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ptr.h"

static int fails = 0;
static int passes = 0;
#define CHECK(cond) do{ \
    if (cond) { \
        printf("PASS: %s\n", #cond); \
        passes++; \
    } \
    else { \
        printf("FAIL: %s\n", #cond); \
        fails++; \
    } \
} while(0)


int main(void) {
    /*
    Extensive testing of functions in ptr.c. 
    1) swap_float: Test with a = NULL, b = NULL, one neg/ one pos, two positive.
    2) minmax: Test with a = NULL, min = NULL, max = NULL, n <= 0, all positive, all negative, mixed values.
    3) findAbove: Test with a = NULL, n <= 0, threshold greater than all values, threshold less than all values, threshold in between values.
    4) reverse: Test with a == NULL, n <= 0, odd number elements, even number elements, and single element.
    */
    
    //TESTING swap_float
    float a_test = 5.1f;
    float b_test = 10.2f;
    float c_test = -3.3f;
    float d_test = -7.4f;

    swap_float(NULL, &b_test); //TEST with a = NULL. Should do nothing.
    CHECK(b_test == 10.2f); // b_test should remain unchanged
    swap_float(&a_test, NULL); //TEST with b = NULL. Should do nothing.
    CHECK(a_test == 5.1f); // a_test should remain unchanged

    swap_float(&a_test, &b_test); //TEST with two positive values. Should swap.
    CHECK(a_test == 10.2f && b_test == 5.1f); // a_test and b_test should be swapped

    swap_float(&c_test, &d_test); //TEST with two negative values. Should swap.
    CHECK(c_test == -7.4f && d_test == -3.3f); // c_test and d_test should be swapped

    //TESTING minmax
    float arr1[] = {3.5f, 2.1f, 6.6f, 44.7f, 20.2f, 16.3f, 1.9f, 33.0f, 24.8f, 5.5f};
    int n1 = sizeof(arr1) / sizeof(float);
    float min1, max1;
    CHECK(minmax(NULL, n1, &min1, &max1) == -1); //TEST with a = NULL. Should return -1.
    CHECK(minmax(arr1, n1, NULL, &max1) == -1); //TEST with min = NULL
    CHECK(minmax(arr1, n1, &min1, NULL) == -1); //TEST with max = NULL
    CHECK(minmax(arr1, 0, &min1, &max1) == -1); //TEST with n <= 0. Should return -1.

    minmax(arr1, n1, &min1, &max1); //TEST with all valid arguments. Should find min and max.
    CHECK(min1 == 1.9f && max1 == 44.7f); // min1 should be 1.9f and max1 should be 44.7f

    //TESTING *findAbove
    float threshold1 = 100.0f;
    float threshold2 = 0.0f; 
    float threshold3 = 20.0f;

    CHECK(findAbove(NULL, n1, threshold1) == NULL); //TEST with a = NULL. Should return NULL.
    CHECK(findAbove(arr1, 0, threshold1) == NULL); //TEST with n <= 0. Should return NULL.
    CHECK(findAbove(arr1, n1, threshold1) == NULL); //TEST with threshold greater than all values. Should return NULL.
    CHECK(findAbove(arr1, n1, threshold2) == &arr1[0]); //TEST with threshold less than all values. Should return pointer to first element.
    CHECK(findAbove(arr1, n1, threshold3) == &arr1[3]); //TEST with threshold in between values. Should return pointer to first value above threshold (44.7f).

    //TESTING reverse
    float arr2[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}; // Odd number of elements
    int n2 = sizeof(arr2) / sizeof(float);

    float arr3[] = {6.0f, 7.0f, 8.0f, 9.0f}; // Even number of elements
    int n3 = sizeof(arr3) / sizeof(float);
    
    float arr4[] = {10.0f}; // Single element
    int n4 = sizeof(arr4) / sizeof(float);

    reverse(NULL, n2); //TEST with a = NULL. Should do nothing.
    reverse(arr2, 0); //TEST with n <= 0. Should do nothing.
    reverse(arr2, n2); //TEST with odd number of elements.
    CHECK(arr2[0] == 5.0f && arr2[1] == 4.0f && arr2[2] == 3.0f && arr2[3] == 2.0f && arr2[4] == 1.0f); // Check if reversed correctly
    reverse(arr3, n3); //TEST with even number of elements.
    CHECK(arr3[0] == 9.0f && arr3[1] == 8.0f && arr3[2] == 7.0f && arr3[3] == 6.0f); // Check if reversed correctly
    reverse(arr4, n4); //TEST with single element. Should do nothing.
    CHECK(arr4[0] == 10.0f); // Check if single element remains unchanged

    //EXPECTED TEST RESULTS: 17 passes, 0 fails. Two tests for reverse give no output, so as long 
    //as code runs without crashing, they are considered passed.
    printf("\nTotal Tests Run: %d\n", passes + fails);
    printf("Total Passes: %d\n", passes);
    printf("Total Fails: %d\n", fails);
        


}