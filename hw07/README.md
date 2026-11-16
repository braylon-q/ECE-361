"make" builds the balance object and its related files. "make test" builds and runs test_istack.c as well as the regress file for inputs/ vs expected/. "make clean" removes all binaries. 

All forms of entry (./balance < $textfile, make test, sh regress.sh) deliver output to the terminal. 


MEMCHECK RESULTS:

1) Destroy same stack twice

==88566== Invalid read of size 8

    Compiler tries to re-take same memory again; undefined behavior.

2) Push onto stack after destroying it

==88702== Invalid read of size 4

    The heap address is still valid so it may still write to that location, i.e. undefined behavior.

3) Leave out istack_destroy

HEAP SUMMARY:
==88792==     in use at exit: 48 bytes in 2 blocks
==88792==   total heap usage: 6 allocs, 4 frees, 5,216 bytes allocated

    Memory is not freed: the data pointer inside of stack s is still allocated as well as the data for the stack itself.
