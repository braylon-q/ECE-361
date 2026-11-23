CC ?= cc
CFLAGS += -std=c11 -Wextra -Wall

.PHONY: all clean memcheck test

all: treestats

treestats: treestats.c bst.c bst.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -o $@ treestats.c bst.c $(LDFLAGS) $(LDLIBS)

test: test_bst
	./test_bst

test_bst: tests/test_bst.c bst.c bst.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -o $@ tests/test_bst.c bst.c $(LDFLAGS) $(LDLIBS)

memcheck: treestats test_bst
	valgrind --leak-check=full --error-exitcode=1 ./treestats
	valgrind --leak-check=full --error-exitcode=1 ./test_bst

clean:
	rm -f treestats test_bst