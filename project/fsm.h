#pragma once

typedef enum {GREEN, YELLOW, RED, WALK, FLASH_DONT_WALK, N_STATES} state_t;
typedef enum {E_NONE, E_BUTTON, E_TIME, E_BOTH, N_EVENTS} event_t;

typedef struct {state_t next;} transition_t; //keep as structure in case extra actions are needed later.

extern const transition_t crossing_table[N_STATES][N_EVENTS];

state_t fsm_next(state_t curr, event_t event);