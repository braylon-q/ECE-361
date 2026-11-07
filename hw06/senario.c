#include "senario.h"
#include <stdio.h>

int senario_load(FILE* input, scenario_t* scenario_in){
    /*
    Load a scenario file in the form of:

        # tick temp rh switches
        0 12.0 70 0x0
        ...

    along with a "start time_t," for an initial timestamp.
    Up to MAX_LINES scenario entries are allowed, written into
    passed scenario_t pointer.

    Ignore comments (#) and empty lines.
    Return 0 on success or the tick of the first failed line.
    */

    //initialize read-in values
    int tick;
    float temp;
    float rh;
    uint32_t switches;
    int line_count = 0;
    //FIXME: add precheck for start timestamp and figure out a way to store it properly.

    while (fscanf(input, "%d %f %f %x", &tick, &temp, &rh, &switches) == 4) {
        if (line_count >= MAX_LINES) {
            return tick; // too many lines
        }
        //assign values
        scenario_in->tick[line_count] = tick;
        scenario_in->temp[line_count] = temp;
        scenario_in->rh[line_count] = rh;
        scenario_in->switches[line_count] = switches;
        line_count++; //increment line count
    }

    return 0; // success
    
}