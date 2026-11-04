#include "sensor.h"
#pragma once

typedef struct store store_t; //Opaque type

typedef struct record {
    reading_t reading;
    int state;
    uint32_t status;
}record_t;

