"make" builds bst.c/.h and treestats.c. "make test" builds bst.c/.h and test_bst.c. "make clean" cleans all generated binaries. "make memcheck" runs valgrind on all files for memory related issues.

**Original search error**

Invalid read of size 4 <-- first memcheck line.
Happened when checking for NULL passthrough of bst_t.

In this scenario, the function never checks for the root of the tree to be NULL. As a result, an error occurs upon trying to dereference a NULL pointer (accessing current->key). The original never noticed this because the array is always filled prior to calling the search function; the root would never be NULL. 

**insertion-order comparison**

$ seq 1 1023 | ./treestats
n 1023, height 1023
$ seq 1 1023 | sort -R | ./treestats
n 1023, height 23

Inserting sorted keys creates a one-sided tree with height 1023. A search can take a full 1023 node visits. The shuffled insertion has a varied height across runs. Search length is limited to the height based on the searching alogorithm.