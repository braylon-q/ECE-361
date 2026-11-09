CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
CPPFLAGS = -MMD -MP

REPLAY_BIN = replay
TEST_BIN = test_hw06
TEST_OBJ = tests/test_hw06.o

COMMON_SRCS = scenario.c sensor.c panel.c iom361_r4.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)
REPLAY_OBJS = main.o $(COMMON_OBJS)
TEST_OBJS = $(TEST_OBJ) $(COMMON_OBJS)
DEPS = $(REPLAY_OBJS:.o=.d) $(TEST_OBJ:.o=.d)

.PHONY: all test clean

all: $(REPLAY_BIN) $(TEST_BIN)

$(REPLAY_BIN): $(REPLAY_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_OBJ): tests/test_hw06.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

test: $(REPLAY_BIN) $(TEST_BIN)
	@status=0; \
	./$(TEST_BIN) || status=1; \
	./regress.sh || status=1; \
	exit $$status

clean:
	rm -f $(REPLAY_BIN) $(TEST_BIN) hw06 test $(TEST_OBJ) $(REPLAY_OBJS) *.d tests/*.d

-include $(DEPS)
