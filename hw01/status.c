#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bits.h"
#include "status.h"

struct status_t status_unpack(uint32_t word) {
    /*
    Extract bits from given word, with specific bit ranges being defined
    as different thermostat fields. Note, the width of the bit reading is 
    the number of bits wanting to be read. Putting in 0 for width will not read 
    any of the status.
    */
    struct status_t thermRead;
    thermRead.heat = get_field(word, 0, 1);
    thermRead.cool = get_field(word, 1, 1);
    thermRead.fan = get_field(word, 2, 1);
    thermRead.fault = get_field(word, 3, 1);
    thermRead.mode = get_field(word, 4, 3);
    thermRead.reserved = get_field(word, 7, 1);

    //Account for the sign when getting the setPoint
    thermRead.setPoint = sign_extend(get_field(word, 8, 8), 8);

    return thermRead;
}

void print_status (struct status_t reading)
{
    //Print out the thermostat status information
    //first, check to make sure the reserved bit is correct
    if (reading.reserved != 0) {
        printf("Error: Reserved bit is not zero\n\n");
        return;
    }

    printf("Temperature Setting: %d Celsius\n", reading.setPoint); // Temperature Print

    switch (reading.mode) //Mode print
    {
        case 0:
            printf("Mode: Off\n");
            break;
        case 1:
            printf("Mode: Heat\n");
            break;
        case 2:
            printf("Mode: Cool\n");
            break;
        case 3:
            printf("Mode: Auto\n");
            break;
        case 4:
            printf("Mode: Fan\n");
            break;
        default:
            printf("Mode: Invalid\n");
            break;
    }

    //if statements for individual status bits
    if (reading.heat) {
        printf("Heat: On\n");
    } else {
        printf("Heat: Off\n");
    }

    if (reading.cool) {
        printf("Cool: On\n");
    } else {
        printf("Cool: Off\n");
    }

    if (reading.fan) {
        printf("Fan: On\n");
    } else {
        printf("Fan: Off\n");
    }

    if (reading.fault) {
        printf("!Fault Detected!\n");
    } 
    printf("\n");
    //End all printing
}