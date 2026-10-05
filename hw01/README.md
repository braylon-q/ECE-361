My library includes 6 functions: 4 bit manipulation functions from part two and 2 functions related to the thermostat problem.

Part 2:

print_binary takes a unsigned 32-bit integer "x" and prints the first "width" number of bits from it, inserting white space every 4 bits. Note width a width of 1 will print the 0th bit of x, meaning it is not directly mappable to the bit's index. A width of 32 will print all 32 bits of x. Any width outside of the range 1 <= width <= 32 will be latched to the upper or lower bound. The final print includes an additional trailing whitespace followed by a newline.

get_field takes an unsigned 32-bit integer "word", integer "pos", integer "width", and returns a uint32_t word[pos] --> word[pos + width-1], shifted down into the lowest "width" number of bits. The function first latches the pos value from 0-31, matching the integer's bit index. Following, width is latched from 1 to 32-pos, ensuring the upper bound of the 32 bit word is not exceeded (i.e. if pos=24, width <= 8).

set_field takes an unsigned 32-bit integer "word", integer "pos", integer "width", unsigned 32-bit integer "value", and returns a uint32_t with bits word[pos] --> word[pos + width-1] replaced with value[0] --> value[pos + width-1]. Error handling for this function is the same as get_field.

sign_extend takes a unsigned 32-bit integer "word", integer "width", and returns the lowest width bits of word converted to a signed 32-bit integer, padding toward MSB with 1's if the bit word[width-1] is a 1. Any width outside of the range 1 <= width <= 32 will be latched to the upper or lower bound.

Part 3:

status_unpack takes a 32-bit integer "word" and stores bits 0-15 in a structure, where the structure maps different bit ranges to thermostat settings. There are 7 members of the structure: heat (bit 0), cool (bit 1), fan (bit 2), fan (bit 3), mode (bits 4-6), reserved (bit 7), and setPoint (bits 8-15). All variables are stores as uint8_t except for setpoint, which is stores as int32_t. An invalid mode or invalid reserved bit will still be stored in status unpack, but are accounted for when printing the thermostat data using print_status. An invalid reserved bit prints a message before stopping the print cycle, while an invalid mode will simply print "Mode: Invalid."

NOTE: For all functions, an unsigned integer input exceeding 32 bits is truncated to the lowest 32 bits of that input. 

Building using VScode will compile files bits.c, status.c and main.c. Running the code through the IDE or its "launch.json" will output the few test cases under main.c. This is only for terminal viewing, as outputs can be harder to interpret through the test_bits.c.

To see and run tests, use the Makefile and WLS. In a connected terminal, run "make clean", which will output 20 test cases alongside the input and "PASS" or "FAIL". Use make clean as needed.
