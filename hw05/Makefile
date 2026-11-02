CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

.PHONY: all clean test run

all: main

main: main.c lock.c lock.h
	$(CC) $(CFLAGS) -o $@ main.c lock.c

tests/test_lock: tests/test_lock.c lock.c lock.h
	$(CC) $(CFLAGS) -I. -o $@ tests/test_lock.c lock.c

test: tests/test_lock
	./tests/test_lock

run: main
	./main

clean:
	rm -f main tests/test_lock