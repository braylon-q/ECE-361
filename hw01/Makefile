CC  = gcc
CFLAGS = -std=c11 -Wall -Wextra -g

OBJS = bits.o status.o

all: $(OBJS)

# Compile .c files into .o files, and account for header changes
%.o: %.c bits.h status.h
	$(CC) $(CFLAGS) -c $< -o $@

#create test_bits object and give access to the header files
tests/test_bits.o: tests/test_bits.c bits.h status.h
	$(CC) $(CFLAGS) -I. -c $< -o $@

# Link the test_bits executable with the object files (not main.c)
tests/test_bits: tests/test_bits.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Run the test_bits executable
test: tests/test_bits
	./tests/test_bits

# Clean up the object files and executables
clean:
	rm -f $(OBJS) tests/test_bits.o tests/test_bits
.PHONY: all test clean


