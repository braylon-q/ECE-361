#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "sensor.h"


int sensor_read(uint32_t *base, float *temp_c, float *rh_pct)
{
    /*
    Reads TEMP_REG and HUMID_REG from the IOM361 device and converts the values to
    Celsius and relative humidity percent. Returns 0 on success and -1 on failure.
    Stores the converted values in the locations pointed to by temp_c and rh_pct.

    // per AHT20 data sheet, Temp(C) = (ST/2**20) * 200 - 50,
    // so ST = (2**20/200) * (Temp(C) + 50)

    // per AHT20 data sheet, RH(%) = (SRH/2**20) * 100,
    // so SRH = (2**20/100) * RH(%)

    NOTE: sizeof(uint32_t) = 4 bytes, so the offsets for TEMP_REG and HUMID_REG are 0x0C (12) and 0x10 (16) respectively.
    */

    //Input error handling: check if base, temp_c, or rh_pct are NULL
    if (base == NULL || temp_c == NULL || rh_pct == NULL) {
        return -1; // Return -1 for failure
    }

    uint32_t sensor_temp_raw = *(base + 0x0C / sizeof(uint32_t)); //raw temp at at index 3
    uint32_t sensor_rh_raw = *(base + 0x10 / sizeof(uint32_t)); //raw humidity at index 4

    // Convert raw values to Celsius and relative humidity percent
    *temp_c = ((float)sensor_temp_raw / 1048576.0f) * 200.0f - 50.0f; // 2**20 = 1048576
    *rh_pct = ((float)sensor_rh_raw / 1048576.0f) * 100.0f; // 2**20 = 1048576

    //round to one decimal place for both values.
    *temp_c = ((int)(*temp_c * 10 + 0.5f)) / 10.0f; //round to one decimal place
    *rh_pct = ((int)(*rh_pct * 10 + 0.5f)) / 10.0f; //round to one decimal place

    return 0; // Return 0 for success
}