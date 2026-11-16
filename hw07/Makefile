CC ?= cc
CFLAGS += -std=c11 -Wall -Wextra

.PHONY: all clean test memcheck

all: balance

balance: balance.c istack.c istack.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ balance.c istack.c $(LDFLAGS) $(LDLIBS)

test: test_istack balance
	./test_istack
	sh regress.sh

memcheck: balance
	valgrind --leak-check=full --error-exitcode=1 ./balance < inputs/balanced.txt

test_istack: tests/test_istack.c istack.c istack.h
	$(CC) -I. $(CPPFLAGS) $(CFLAGS) -o $@ tests/test_istack.c istack.c $(LDFLAGS) $(LDLIBS)

clean:
	rm -f balance test_istack