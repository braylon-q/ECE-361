#pragma once

struct status_t {
    uint8_t heat;
    uint8_t cool;
    uint8_t fan;
    uint8_t fault;
    uint8_t mode;
    uint8_t reserved;
    int32_t setPoint;
};

struct status_t status_unpack(uint32_t word);

void print_status (struct status_t reading);