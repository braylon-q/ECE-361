CC ?= gcc
CFLAGS := -std=c11 -Wall -Wextra

SRCS := main.c aux.c fsm.c report.c sensor.c store.c iom361_r4.c
OBJS := $(SRCS:.c=.o)
TARGET := crossing

.PHONY: all run clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
