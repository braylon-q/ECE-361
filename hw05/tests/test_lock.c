#include <stdio.h>
#include "lock.h"

/* Count each assertion and print its result and descriptive label. */
#define CHECK(name, cond) do { \
    total++; \
    if (cond) { \
        printf("PASS: %s\n", name); \
        passes++; \
    } \
    else { \
        printf("FAIL: %s\n", name); \
        fails++; \
    } \
} while (0)

/* Apply input characters through the FSM and return the final state. */
static state_t run_sequence(const char *input)
{
    /* Each test sequence starts with the lock secured. */
    state_t state = LOCKED;

    while (*input != '\0') {
        /* Convert the next character into an event for the current state. */
        int event = lock_event(state, *input++);
        action_t action = A_UNDEFINED;

        /* Invalid characters make the sequence invalid. */
        if (event < 0)
            return (state_t)-1;

        /* Apply the event and track the resulting FSM state. */
        state = lock_step(state, (event_t)event, &action);
        if (state < 0)
            return (state_t)-1;
    }

    return state;
}

int main(void) {
    /* Expected next state and action for all 16 state/event combinations. */
    static const transition_t expected[N_STATES][N_EVENTS] = {
        [LOCKED] = {
            {GOT1, A_NONE}, {LOCKED, A_BEEP},
            {LOCKED, A_NONE}, {LOCKED, A_NONE}
        },
        [GOT1] = {
            {GOT2, A_NONE}, {LOCKED, A_BEEP},
            {GOT1, A_NONE}, {LOCKED, A_NONE}
        },
        [GOT2] = {
            {OPEN, A_UNLOCK}, {LOCKED, A_BEEP},
            {GOT2, A_NONE}, {LOCKED, A_NONE}
        },
        [OPEN] = {
            {OPEN, A_NONE}, {OPEN, A_NONE},
            {LOCKED, A_LOCK}, {OPEN, A_NONE}
        }
    };
    static const char *const state_names[N_STATES] = {
        "LOCKED", "GOT1", "GOT2", "OPEN"
    };
    static const char *const event_names[N_EVENTS] = {
        "RIGHT", "WRONG", "CLOSE", "TIMEOUT"
    };
    /* The correct digit expected in each state, including OPEN. */
    static const char right_digits[N_STATES] = {'3', '1', '4', '3'};
    /* A digit that must be classified as wrong in each state. */
    static const char wrong_digits[N_STATES] = {'0', '0', '0', '1'};

    /* Track the total, passing, and failing assertion counts. */
    int passes = 0;
    int fails = 0;
    int total = 0;

    /* Tests 1-16: check every transition-table next state and action. */
    for (state_t state = LOCKED; state < N_STATES; state++) {
        for (event_t event = E_RIGHT; event < N_EVENTS; event++) {
            action_t actual_action = A_UNDEFINED;
            state_t actual_next = lock_step(state, event, &actual_action);
            char test_name[64];

            (void)snprintf(test_name, sizeof test_name, "table %s / %s",
                           state_names[state], event_names[event]);
            /* Verify both fields of this state's transition for this event. */
            CHECK(test_name,
                  actual_next == expected[state][event].next &&
                  actual_action == expected[state][event].action);
        }
    }

    /* Tests 17-36: verify digit and control-character decoding in each state. */
    for (state_t state = LOCKED; state < N_STATES; state++) {
        char test_name[64];

        /* Verify that this state's expected digit is RIGHT. */
        (void)snprintf(test_name, sizeof test_name, "lock_event %s right digit",
                       state_names[state]);
        CHECK(test_name, lock_event(state, right_digits[state]) == E_RIGHT);

        /* Verify that a different digit is WRONG. */
        (void)snprintf(test_name, sizeof test_name, "lock_event %s wrong digit",
                       state_names[state]);
        CHECK(test_name, lock_event(state, wrong_digits[state]) == E_WRONG);

        /* Verify that 'c' is recognized as the door-close event. */
        (void)snprintf(test_name, sizeof test_name, "lock_event %s c",
                       state_names[state]);
        CHECK(test_name, lock_event(state, 'c') == E_CLOSE);

        /* Verify that 't' is recognized as the timeout event. */
        (void)snprintf(test_name, sizeof test_name, "lock_event %s t",
                       state_names[state]);
        CHECK(test_name, lock_event(state, 't') == E_TIMEOUT);

        /* Verify that an unsupported character is rejected. */
        (void)snprintf(test_name, sizeof test_name, "lock_event %s other character",
                       state_names[state]);
        CHECK(test_name, lock_event(state, 'x') == -1);
    }

    /* Test 37: the correct code sequence moves the FSM to OPEN. */
    CHECK("sequence 3 1 4 opens", run_sequence("314") == OPEN);
    /* Test 38: an incorrect digit prevents the sequence from opening. */
    CHECK("sequence 3 3 1 4 does not open", run_sequence("3314") != OPEN);
    /* Test 39: timeout interrupts entry and prevents opening. */
    CHECK("sequence 3 1 t 4 does not open", run_sequence("31t4") != OPEN);
    /* Test 40: closing after opening returns the FSM to LOCKED. */
    CHECK("sequence 3 1 4 c locks again", run_sequence("314c") == LOCKED);

    /* Print the final result and return failure status if any check failed. */
    printf("\nSummary: %d tests, %d passed, %d failed\n", total, passes, fails);
    return fails == 0 ? 0 : 1;
}