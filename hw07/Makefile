CC ?= cc
CFLAGS += -std=c11 -Wall -Wextra

.PHONY: all clean test

all: balance

balance: balance.c istack.c istack.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ balance.c istack.c $(LDFLAGS) $(LDLIBS)

test: test_istack
	./test_istack

test_istack: tests/test_istack.c istack.c istack.h
	$(CC) -I. $(CPPFLAGS) $(CFLAGS) -o $@ tests/test_istack.c istack.c $(LDFLAGS) $(LDLIBS)

clean:
	rm -f balance test_istack