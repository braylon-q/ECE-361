#include "fsm.h"

const transition_t crossing_table[N_STATES][N_EVENTS] = {
    /*                  E_NONE              E_TIME             E_BUTTON           E_BOTH */
    [GREEN]           = {{GREEN},           {YELLOW},          {GREEN},           {YELLOW},},
    [YELLOW]          = {{YELLOW},          {YELLOW},          {RED},             {RED}},
    [RED]             = {{RED},             {RED},             {WALK},            {WALK}},
    [WALK]            = {{WALK},            {WALK},            {FLASH_DONT_WALK}, {FLASH_DONT_WALK}},
    [FLASH_DONT_WALK] = {{FLASH_DONT_WALK}, {FLASH_DONT_WALK}, {GREEN},           {GREEN}}
};

state_t fsm_next(state_t curr, event_t event) {
    /*
    Return the next state based on the current state and event.
    */
    return crossing_table[curr][event].next;
}
