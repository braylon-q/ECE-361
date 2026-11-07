#include <stdio.h>
#include "panel.h"

uint32_t panel_pressed(uint32_t now,uint32_t before){
    /*
    Switches that went from off to on, one bit per switch.
    returns uint32_t. Single line statement.
    */
    return now & ~before;
}

uint32_t panel_released(uint32_t now, uint32_t before){
    /*
    Switches that went from on to off, one bit per switch.
    returns uint32_t. Single line statement.
    */
    return before & ~now;
}

int panel_led(uint32_t *base, int n, int on)
{
    /*
    Turns LED n on or off using RMW. Every other LED is preserved.
    Returns 0 if successful. -1 if n is out of range.
    */ 
    if (n < 0 || n >= 32) {
        return -1; //Check invalid values.
    }

    if (on) {
        *base |= 1U << n;
    } else {
        *base &= ~(1U << n);
    }

    return 0;
}

int panel_toggle(uint32_t *base, int n){
    /*
    Toggles LED n using RMW. Every other LED is preserved.
    Returns 0 if successful. -1 if n is out of range.
    */ 
    if (n < 0 || n >= 32) {
        return -1; //Check invalid values.
    }

    *base ^= 1U << n;

    return 0;
}


