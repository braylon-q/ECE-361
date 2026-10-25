To build the events.c file, run "make" in a connected terminal. View events.c output by then running "./events < data/door.txt". Notice that neither empty lines or lines starting with # are counted as skips, as these are cleaned up by events.c before passing through to tally_add(). Skips are only counted for data passed to tally_add() that is not added.

To test, run "make test" in connected terminal. Output will show the 16 test cases, lead by a "PASS" or "FAIL" to indicate whether that specific test was true. A summary prints at the bottom of the test file with total passes, fails, and total passes+fails. 

"make clean" removes all objects.

Running "t->size" inside of a file that includes tally.h gives the following output:

invalid use of incomplete typedef ‘tally_t’ {aka ‘struct tally’}

The opaque type prevents files that include a given header file but do not define it's opaque type from accessing what is inside of that opaque type. In this instance, events.c and test_tally.c know the structure exists, but cannot access any of its elements. This is useful for many reasons. It may be we do not want the caller to be able to modify any of the data inside the structure, potentially improperly. It also makes the opaque type more secure; callers won't understand how it works, and therefore can't exploit it.



