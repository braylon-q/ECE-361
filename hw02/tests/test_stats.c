#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stats.h"


static int fails = 0;
#define CHECK(cond) do{ \
    if (cond) { \
        printf("PASS: %s\n", #cond); \
    } \
    else { \
        printf("FAIL: %s\n", #cond); \
        fails++; \
    } \
} while(0)


int main() {

    /*TEST 1: array value is equal to threshold, which ends a run.
    EXPECTED: best should be 2. 
    */
    float test_array[] = {1.0f, 2.0f, 2.0f, 1.9f};
    int count = sizeof(test_array) / sizeof(test_array[0]);
    float threshold = 1.9f;
    bestRun result = stats_threshold(test_array, count, threshold);
    CHECK(result.best == 2);
    printf("TEST 1: best run above threshold %.1f: %d\n\n", threshold, result.best);

    /*
    TEST 2: Maximum value of array is at the beginning of the array.
    EXPECTED: 99.9
    */
    float test_array2[] = {99.9f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    count = sizeof(test_array2) / sizeof(test_array2[0]);
    stats result2 = stats_fill(test_array2, count);
    CHECK(result2.max == 99.9f);
    printf("TEST 2: maximum value in array[0]: %.1f\n\n", result2.max);

    /*
    Test 3: Maximum value of array is at the end of the array.
    EXPECTED: 99.8
    */
    float test_array3[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 99.8f};
    count = sizeof(test_array3) / sizeof(test_array3[0]);
    stats result3 = stats_fill(test_array3, count);
    CHECK(result3.max == 99.8f);
    printf("TEST 3: maximum value in array[last]: %.1f\n\n", result3.max);

    /*
    TEST 4: Maximum value of array is outside recursion limit. Max recursion = 9999 single-function frames.
    EXPECTED: 5.0f.
    */
   float test_array4[10000] = {0};
   test_array4[0] = 5.0f;
   test_array4[9999] = 99.7f;
   count = sizeof(test_array4) / sizeof(test_array4[0]);
   stats result4 = stats_fill(test_array4, count);
   CHECK(result4.max == 5.0f);
   printf("TEST 4: maximum value in array (outside recursion limit): %.1f\n\n", result4.max);

   return fails != 0;
}