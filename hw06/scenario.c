#include "scenario.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

int scenario_load(FILE* input, scenario_t* scenario_in, time_t *start_time){
    /*
    Load a scenario file in the form of:

        start <timestamp>
        # tick temp rh switches
        0 12.0 70 0x0
        ...

    along with a "start time_t," for an initial timestamp.
    Up to MAX_LINES scenario entries are allowed, written into
    passed scenario_t pointer.

    Ignore comments (#) and empty lines.
    Return 0 on success or the tick of the first failed line.
    */
    if (input == NULL || scenario_in == NULL || start_time == NULL) {
        return 1; //invalid function inputs return 1
    }


    uint32_t tick;
    float temp;
    float rh;
    uint32_t switches;
    char line[256];
    int seen_start = 0;

    scenario_in->size = 0;

    while (fgets(line, sizeof(line), input) != NULL) {
        char* cursor = line;
        //Skip invalid characters by moving forward along string
        while (*cursor == ' ' || *cursor == '\t' || *cursor == '\r' || *cursor == '\n') {
            cursor++;
        }

        //Change lines on NULL chars and comments
        if (*cursor == '\0' || *cursor == '#') {
            continue;
        }

        if (!seen_start) {
            /*
            Search for the "start <time_t>" line, ensure the correct keyword, and then
            store the timestamp in start_time. 
            */
            char keyword[32];
            long long read_start = 0;
            if (sscanf(cursor, "%31s %lld", keyword, &read_start) != 2) {
                return 1;
            }
            if (strcmp(keyword, "start") != 0) {
                return 1;
            }
            *start_time = (time_t) read_start;
            seen_start = 1;
            continue;
        }

        //Invalid line. Return tick per spec.
        if (sscanf(cursor, "%u %f %f %x", &tick, &temp, &rh, &switches) != 4) {
            return tick;
        }

        //Return tick if too many lines.
        if (scenario_in->size >= MAX_LINES) {
            return tick; // too many lines
        }

        //Return tick if scenario entries do not increase.
        if (scenario_in->size > 0 && tick <= scenario_in->tick[scenario_in->size - 1]) {
            return tick;
        }

        //Assign values to scenario arrays.
        scenario_in->tick[scenario_in->size] = tick;
        scenario_in->temp[scenario_in->size] = temp;
        scenario_in->rh[scenario_in->size] = rh;
        scenario_in->switches[scenario_in->size] = switches;
        scenario_in->size++;
    }

    //Return 1 if start line was not found.
    if (!seen_start) {
        return 1;
    }

    return 0;
}