CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

TARGET = main
TEST_TARGET = tests/test_ptr

.PHONY: all test clean

all: $(TARGET)

$(TARGET): main.c ptr.c ptr.h
	$(CC) $(CFLAGS) -I. -o $@ main.c ptr.c

$(TEST_TARGET): tests/test_ptr.c ptr.c ptr.h
	$(CC) $(CFLAGS) -I. -o $@ tests/test_ptr.c ptr.c

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)
