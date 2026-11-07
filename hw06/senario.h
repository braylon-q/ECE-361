#pragma once
#include <stdio.h>
#include <stdint.h>
#define MAX_LINES 1000


int senario_load(FILE* input, scenario_t* scenario_in);
/*
Load a scenario file in the form of:

    # tick temp rh switches
    0 12.0 70 0x0

along with a "start time_t," for an initial timestamp.
Up to MAX_LINES scenario entries are allowed, stored as a array
of scenario_t.

Ignore comments (#) and empty lines.
Return 0 on success or the tick of the first failed line.
*/

typedef struct scenario {
    int tick[MAX_LINES]; //tick number in the scenario
    float temp[MAX_LINES]; //temperature in celsius
    float rh[MAX_LINES]; //relative humidity in percent
    uint32_t switches[MAX_LINES]; //switch states in the scenario
} scenario_t;