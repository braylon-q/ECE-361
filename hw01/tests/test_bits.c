#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "bits.h"
#include "status.h"



static int saved_fd;
static FILE *cap;

static int fails = 0;
//static int manualChecks = 0;

#define CHECK(cond) do{ \
    if (cond) { \
        printf("PASS: %s\n", #cond); \
    } \
    else { \
        printf("FAIL: %s\n", #cond); \
        fails++; \
    } \
} while(0)

//#cond is a preprocessor macro that converts the condition to a string
// \ is a line continuation character in C preprocessor macros

static void capture_start(void) {
    fflush(stdout);
    saved_fd = dup(STDOUT_FILENO);          // remember the real terminal
    cap = tmpfile();                        // temporary file to catch output
    dup2(fileno(cap), STDOUT_FILENO);       // point stdout at it
}

static void capture_stop(char *buf, size_t n) {
    fflush(stdout);
    dup2(saved_fd, STDOUT_FILENO);          // restore the terminal
    close(saved_fd);
    rewind(cap);
    size_t len = fread(buf, 1, n - 1, cap); // read what was printed
    buf[len] = '\0';
    fclose(cap);
}

int main(void) {
    /*TESTS: print_binary; test 1 checks for width below boundry,
    test 2 checks for width above boundry, and test 3 prints bits first 17 bits
     of 0x12345678*/
    char out1[256];
    capture_start();
    print_binary(0xA5A5A5A5, 0); // EXPECTED: 1
    capture_stop(out1, sizeof(out1));
    CHECK(strcmp(out1, "1 \n") == 0);

    char out2[256];
    capture_start();
    print_binary(0xA5A5A5A5, 33); // EXPECTED: 1010 0101 1010 0101 1010 0101 1010 0101
    capture_stop(out2, sizeof(out2));
    CHECK(strcmp(out2, "1010 0101 1010 0101 1010 0101 1010 0101 \n") == 0);
    
    char out3[256];
    capture_start();
    print_binary(0x12345678, 17); // EXPECTED: 0 0101 0110 0111 1000
    capture_stop(out3, sizeof(out3));
    CHECK(strcmp(out3, "0 0101 0110 0111 1000 \n") == 0);


    /*TESTS: get_field; test 1 checks lower bound of pos, 
    test 2 checks upper bound of pos, test 3 checks pos+width value latching. 
    uses 0xA5A5A5A5 */
    CHECK(get_field(0xA5A5A5A5, -1, 8) == 0xA5);
    CHECK(get_field(0xA5A5A5A4, 34, 1) == 0x1); //4 used at end of these lines
    CHECK(get_field(0xA5A5A5A4, 31, 2) == 0x1); //to ensure proper replacement

    /*TESTS: set_field; test 1 checks lower bound of pos, 
    test 2 checks upper bound of pos, test 3 checks pos+width value latching, test 4
    checks bits 16-23 replaced. 
    uses 0xA5A5A5A5 for word and 0x0FFFFFF0 for value */
    CHECK(set_field(0xA5A5A5A5, -1, 8, 0x0FFFFFF0) == 0xA5A5A5F0);
    CHECK(set_field(0xA5A5A5A5, 34, 1 ,0x0FFFFFF0) == 0x25A5A5A5);
    CHECK(set_field(0xA5A5A5A5, 31, 2, 0x0FFFFFF0) == 0x25A5A5A5);
    CHECK(set_field(0xA5A5A5A5, 16, 8, 0x0FFFFFF0) == 0xA5F0A5A5);

    /*TESTS: sign_extend; test 1 checks lower bound of width, test 2 checks
    upper bound of width, test 3 checks output of max unsigned value: 0xFFFFFFFF,
    test 4 checks output of min signed value: 0x80000000 */
    CHECK(sign_extend(0xA5A5A5A5, 0) == (int32_t)0xFFFFFFFF);
    CHECK(sign_extend(0xA5A5A5A5, 33) == (int32_t)0xA5A5A5A5);
    CHECK(sign_extend(0xFFFFFFFF, 8) == (int32_t)0xFFFFFFFF);
    CHECK(sign_extend(0x80000000, 32) == (int32_t)0x80000000);

    /*TESTS: status_unpack: Test 1 checks 0x1631 for temperature == 22 C, 
    test 2 checks 0x1480 for invalid reserved bit, and test 3 checks 0xAF73 for
    an invalid mode selection*/
    struct status_t status1 = status_unpack(0x1631);
    CHECK(status1.setPoint == 22);

    struct status_t status2 = status_unpack(0x1480);
    CHECK(status2.reserved == 1);

    struct status_t status3 = status_unpack(0xAF73);
    CHECK(status3.mode == 7);

    /*TESTS: status_print; matches a printed output to the above tests,
    must MANUALLY ensure output is correct.*/
    char out4[256];
    capture_start();
    print_status(status1);
    capture_stop(out4, sizeof(out4));
    CHECK(strcmp(out4, "Temperature Setting: 22 Celsius\nMode: Auto\nHeat: On\nCool: Off\nFan: Off\n\n") == 0);
    
    char out5[256];
    capture_start();
    print_status(status2);
    capture_stop(out5, sizeof(out5));
    CHECK(strcmp(out5, "Error: Reserved bit is not zero\n\n") == 0);

    char out6[256];
    capture_start();
    print_status(status3);
    capture_stop(out6, sizeof(out6));
    CHECK(strcmp(out6, "Temperature Setting: -81 Celsius\nMode: Invalid\nHeat: On\nCool: On\nFan: Off\n\n") == 0);


    return fails != 0; //return 1 if there are any failures
}