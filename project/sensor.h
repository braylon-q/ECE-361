#pragma once

int sensor_read(uint32_t *base, float *temp_c, float *rh_pct);
/*
Reads TEMP_REG and HUMID_REG from the IOM361 device and converts the values to
Celsius and relative humidity percent, rounded to one decimal place. Returns 0 on success and -1 on failure.
Stores the converted values in the locations pointed to by temp_c and rh_pct.
*/