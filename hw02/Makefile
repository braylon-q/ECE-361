CC = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -Wpedantic

MAIN_SOURCES = histogram.c readings.c stats.c main.c
TEST_STATS_SOURCES = tests/test_stats.c stats.c

.PHONY: all test test_stats regress clean

all: main

main: $(MAIN_SOURCES) histogram.h readings.h stats.h
	$(CC) $(CFLAGS) $(MAIN_SOURCES) -o $@

tests/test_stats: $(TEST_STATS_SOURCES) stats.h
	$(CC) $(CFLAGS) $(TEST_STATS_SOURCES) -o $@

test_stats: tests/test_stats
	./tests/test_stats

regress: main
	sh regress.sh

test:
	$(MAKE) test_stats
	$(MAKE) regress

clean:
	rm -f main tests/test_stats