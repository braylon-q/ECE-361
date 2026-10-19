#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "aux.h"
#include "fsm.h"
#include "report.h"
#include "sensor.h"
#include "store.h"
#include "iom361_r4.h"

int main(void) {
    int rc = 0;
    uint32_t *base = iom361_initialize(1, 4, &rc);

    if (base == NULL) {
        fprintf(stderr, "iom361_initialize failed: %d\n", rc);
        return 1;
    }
    printf("Pedestrian Crossing\n");

    //WEEK 2: temperature and humidity sensor.
    _iom361_set_sensor1(30.124f, 50.999f);
    float temp, humid;
    rc = sensor_read(base, &temp, &humid);
    if (rc != 0) {
        printf("sensor_read failed: %d\n", rc);
        return 1;
    }
    printf("Temperature: %f°C, Humidity: %f %%RH\n", temp, humid);

    return 0;
}