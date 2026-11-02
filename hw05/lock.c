#include <stdio.h>
#include <stdlib.h>
#include "lock.h"

const transition_t lock_table[N_STATES][N_EVENTS] = {
    /*          E_RIGHT          E_WRONG           E_CLOSE          E_TIMEOUT */
    [LOCKED] = {{GOT1, A_NONE}, {LOCKED, A_BEEP}, {LOCKED, A_NONE}, {LOCKED, A_NONE}},
    [GOT1]   = {{GOT2, A_NONE}, {LOCKED, A_BEEP}, {GOT1, A_NONE},   {LOCKED, A_NONE}},
    [GOT2]   = {{OPEN, A_UNLOCK}, {LOCKED, A_BEEP}, {GOT2, A_NONE}, {LOCKED, A_NONE}},
    [OPEN]   = {{OPEN, A_NONE}, {OPEN, A_NONE}, {LOCKED, A_LOCK},   {OPEN, A_NONE}}
};

state_t lock_step(state_t s, event_t e, action_t *action){
    /*
    One line lookup for next state. Changes action pointer. 
    Returns next state. Invalid input returns previous state.
    */
    if (s < LOCKED || s >= N_STATES || e < E_RIGHT || e >= N_EVENTS || action == NULL)
        return -1; //check for invalid function inputs. -1

   transition_t next = lock_table[s][e];
   *action = next.action;
   return next.next;
}

int lock_event(state_t s, char c){
    /*
    Turns character into event for given state.
    
    A digit is E_RIGHT if it is the next digit of the code, otherwise E_WRONG; c is the
    door closing, t the timer running out; anything else −1.
    */

    if (s < LOCKED || s >= N_STATES)
        return -1;

    if (c == 'c')
        return E_CLOSE;
    if (c == 't')
        return E_TIMEOUT;
    if (c < '0' || c > '9')
        return -1; //ASCII char check

    switch (s) {
        case LOCKED:
            return c == '3' ? E_RIGHT : E_WRONG;
        case GOT1:
            return c == '1' ? E_RIGHT : E_WRONG;
        case GOT2:
            return c == '4' ? E_RIGHT : E_WRONG;
        case OPEN:
            return c == '3' ? E_RIGHT : E_WRONG;
        default:
            return -1;
    }
}

const char *state_name(state_t s){
    /*
    Gives state_name as a char* for a given input. 
    Invalid input handled by return. 
    */
    static const char *state_chars[N_STATES] = {"LOCKED", "GOT1", "GOT2", "OPEN"};
    //Static array of char*. Must be static to avoid dangling pointer.

    return s >= LOCKED && s < N_STATES ? state_chars[s] : "?"; //return name of state or "?" if invalid. 
} 

const char *event_name(event_t e){
    /*
    Gives event_name as a char* for a given input. 
    Invalid input handled by return 
    */
    static const char *event_chars[N_EVENTS] = {"RIGHT", "WRONG", "CLOSE", "TIMEOUT"};
    //Static array of char*. Must be static to avoid dangling pointer.

    return e >= E_RIGHT && e < N_EVENTS ? event_chars[e] : "?";//return name of event or "?" if invalid
} 


const char *action_name(action_t a){
    /*
    Gives action_name as a char* for a given input. 
    Invalid input handled by return 
    */
    static const char *action_chars[N_ACTIONS] = {"UNDEFINED", "NONE", "UNLOCK", "LOCK", "BEEP"};
    //Static array of char*. Must be static to avoid dangling pointer.

    return a >= A_UNDEFINED && a < N_ACTIONS ? action_chars[a] : "?"; //return name of action or "?" if invalid
} 

