CC = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -Wpedantic

.PHONY: all test clean

all: events

events: events.c tally.c tally.h
	$(CC) $(CFLAGS) events.c tally.c -o events

test_tally: tests/test_tally.c tally.c tally.h
	$(CC) $(CFLAGS) tests/test_tally.c tally.c -o test_tally

test: test_tally
	./test_tally

clean:
	rm -f events test_tally
