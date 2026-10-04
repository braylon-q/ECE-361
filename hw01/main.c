#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bits.h"
#include "status.h"

int main(void) {

    //BEGIN BITS.C TESTS

    //TEST 1: extract 8 bits from 0x78FF (7 and 8) and then print result with
    //Print_binary. Expected output: 0000 0111 1000
    uint32_t result = get_field(0x78FF, 8, 8);
    print_binary(result, 12);

    //TEST 2: Replace bits 8-15 with 0xFF inside of target 20-bit word.
    //then print_binary. Expected from 0x695AB: 0110 1111 1111 1010 1011
    uint32_t target = 0x695AB;
    uint32_t result2 = set_field(target, 8, 8, 0xAAFF);
    print_binary(result2, 20);

    //TEST 3: sign extend lowest 12 bits of 0x0FFE to get an unsigned
    //integer to an int32_t and print it. Expected output: -2
    uint32_t unsigned_value = 0x0FFE;
    int32_t result3 = sign_extend(unsigned_value, 12);
    printf("%d\n", result3);

    //END BITS.C TESTS
    
    //BEGIN STATUS.C TESTS
    //TEST 1: Given test by instructor documentation
    struct status_t currentStatus = status_unpack(0x1631);
    print_status(currentStatus); 

    //END STATUS.C TESTS


    return 0;
}