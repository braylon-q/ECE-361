#pragma once
#include <stdint.h>

uint32_t panel_pressed(uint32_t now,uint32_t before);
/*
Switches that went from off to on, one bit per switch.
*/

uint32_t panel_released(uint32_t now, uint32_t before);
/*
Switches that went from on to off, one bit per switch.
*/

int panel_led(uint32_t *base, int n, int on);
/*
Turns LED n on or off. Every other LED is preserved.
Returns 0 if successful. -1 if n is out of range.
*/ 

int panel_toggle(uint32_t *base, int n);
/*
Toggles LED n. Every other LED is preserved.
Returns 0 if successful. -1 if n is out of range.
*/ 