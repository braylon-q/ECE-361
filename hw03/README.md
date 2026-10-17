














LIBRARY FUNCTIONS: 

swap_float takes two float pointers and swaps the values they point to. Does nothing if either pointer is NULL. Returns nothing.

minmax takes 4 arguments: const float pointer to an array, an integer array length, a float pointer for max and a float pointer for min. Returns 0 if min and max are filled successfully. If any pointer is NULL or the array has a length less than 1, it returns -1.

findAbove takes 3 arguemnts: const float pointer to an array, an integer array length, and a float threshold value. Returns pointer to first value in array above threshold. If any pointer is NULL, array has length less than 1, or no value exists above threshold, returns NULL.

reverse takes a const float pointer to an array and an integer array length. Returns nothing, but reverses all values in array. If any pointer is null orr array has length less than 1, it does nothing.

BUILDING AND TESTING:

Debugging via VScode with gcc will run main.c, printing 1 output for each function for manual verification of functions. Running "make" in terminal builds main and subsidiary .c/.h files. Running "make test" builds and runs test_ptr, building subsidiary functions beforehand as well. "make clean" removes what was built.