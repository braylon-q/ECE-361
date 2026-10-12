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

    return 0;
}