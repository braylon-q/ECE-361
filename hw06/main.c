#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "iom361_r4.h"
#include "scenario.h"
#include "panel.h"
#include "sensor.h"

int main(int argc, char * argv[]) {
    //Check command line arguments: account for 
    //entry differences

    FILE *input = stdin;
    if (argc > 2) {
        printf("Too many command line arguments\n");
        return 1;
    }
    else if (argc == 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            printf("Failed to open input file\n");
            return 1;
        }
    }

    int rc = 0;
    uint32_t *base = iom361_initialize(4, 4, &rc); //initialize with 4 switches and 4 leds
    if (rc == 1) {
        printf("Initialization failed\n");
        return rc;
    }

    iom361_set_display(false); //initialize display as off
    //Return code only NULL if passed through as NULL or not included)
    //Initialization checks done.
    time_t start;
    scenario_t scene;

    rc = scenario_load(input, &scene, &start); //load scenario text file
    if (rc != 0) {
        printf("Scenario load failed\n");
        return rc; //return on failed load
    }

    /* Loop for scenario entry

    For every given tick inside of a scenario:
        Set the emulated world temperature, humidity, and switches using _iom361_set_sensor1
            and _iom361_set_switches.
        Then, through the registers, read sensor + switches, toggling LED #n for every switch #n
            pressed.
        Print out a line matching the following format:
            <start + tick>      <temp>    <rh>    <switches>   <pressed>   <leds>
            2026-10-06 08:00:02 temp=21.5 rh=40.0 switches=0x1 pressed=0x1 leds=0x1

    */
    uint32_t previous_switches = 0;
    uint32_t leds = 0;

    for (int i = 0; i < scene.size; i++) {
        _iom361_set_sensor1(scene.temp[i], scene.rh[i]);
        _iom361_set_switches(scene.switches[i]);

        float temp = 0.0f;
        float rh = 0.0f;
        if (sensor_read(base, &temp, &rh) != 0) {
            printf("Sensor read failed\n");
            return -1;
        }

        uint32_t switches = iom361_readReg(base, SWITCHES_REG, &rc);
        if (rc != 0) {
            printf("Switch read failed\n");
            return rc;
        }

        uint32_t pressed = panel_pressed(switches, previous_switches);

        //Toggle switched leds
        for (int bit = 0; bit < 32; bit++) {
            if ((pressed & (1U << bit)) != 0U) {
                panel_toggle(&leds, bit);
            }
        }

        iom361_writeReg(base, LEDS_REG, leds, &rc);
        if (rc != 0) {
            printf("LED write failed\n");
            return rc;
        }

        time_t tick_time = start + (time_t) scene.tick[i];
        struct tm *tm_info = gmtime(&tick_time);
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info); //Organize timestamp for printing

        //print out formatted data
        printf("%s temp=%.1f rh=%.1f switches=0x%x pressed=0x%x leds=0x%x\n",
               timestamp,
               temp,
               rh,
               (unsigned int) switches,
               (unsigned int) pressed,
               (unsigned int) leds);

        //Update previous switches for next iteration
        previous_switches = switches;
    }


    return 0;
}