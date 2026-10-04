#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bits.h"

//FIXME: ALl of these tasks are done. Handle edge cases before submitting.


void print_binary(uint32_t x, int width) 
{
    /*
    Current plan: Print binary by bit masking bit by bit 
    using & and then shifiting over. Once limit is reached, stop.
    Add a space very 4 bits
    */

    //ERROR CORRECTION: Latch width from 1-32
    if (width < 1) width = 1;
    if (width > 32) width = 32;

    uint32_t mask = 1u;

    for (int i = width - 1; i>=0; i--) {
        mask = 1u << i;
        if (x & mask) //check if number is positive. A positive number means a 1 at current position
        {
            printf("1");
        }
        else
        {
            printf("0");//otherwise, number is 0
        }
        if (i % 4 == 0){
            printf(" "); //Print blank space every 4 bits
        }
   }
   printf("\n"); //Print newline at end. Prepare for next output.
  
}

uint32_t get_field(uint32_t word, int pos, int width)
{
    /*
    Plan: shift right pos bits to get bits in LSB position.
    Then, use for loop to load serial mask, then apply it 
    to extract the desired field.
    */
    //ERROR CORRECTION: Latch pos from 0-31
    if (pos < 0) pos = 0;
    if (pos > 31) pos = 31;
    //ERROR CORRECTION: Latch width so that pos+width <= 32 and width >= 1
    if (width < 1) width = 1;
    if (pos + width > 32) width = 32 - pos;


    word >>= pos; //Shift right to correct position
    uint32_t mask = 0;
    for (int i = 0; i < width; i++) {
        mask |= (1u << i); //load bits into mask (e.g. 5 bit width = ..0001 1111)
    }
    return word & mask; //bitwise AND to extract the desired field and return

}

uint32_t set_field(uint32_t word, int pos, int width, uint32_t value)
{
    /*
    PLAN: We want to replace a subsection of word defined from
    pos to pos + width - 1 with the lowest (closest to LSB) bits
    of an arbitrary integer value. If Value > 32 bits, only the lowest
    32 bits will be used.

    First, we will create a mask to clear bits from pos to pos + width - 1.
    Then, we will shift the correct # 1's into our mask.
    After, we will shift the mask over to the correct position to assert the target bits.
    Following, we will apply the mask to the word.
    Next, we will shift the value left by pos to align it with the target position.
    Finally, we will combine the masked word and the shifted value using bitwise OR to produce the result.
    */
    
    //ERROR CORRECTION: Latch pos from 0-31
    if (pos < 0) pos = 0;
    if (pos > 31) pos = 31;
    //ERROR CORRECTION: Latch width so that pos+width <= 32 and width >= 1
    if (width < 1) width = 1;
    if (pos + width > 32) width = 32 - pos;


    uint32_t mask = 0;
    for (int i = 0; i < width; i++) {
        mask |= (1u << i); //load bits into mask (e.g. 5 bit width = ..0001 1111)
    }
    mask <<= pos; //shift mask to correct position
    word &= ~mask; //clear target bits in word 
    value <<= pos;

    //CHANGE: we will have to clear all bits outside of target range in value. 
    value &= mask; //clear all bits outside of target range in value
    return word | value; //combine the masked word and the shifted value using bitwise OR to produce the result.
    
}

int32_t sign_extend(uint32_t value, int width)
{
    /*
    GOAL: Take lowest width bits of an unsigned integer value
    and sign-extend them to an int32_t, then printing out the decimal
    number.

    PLAN: Create mask to extract lowest width bits of our value.
    Then, we will check if the most significant bit of the extracted field is 1.
    If yes, we will sign-extend the value by OR'ing all bits with an inverted mask. 
    Then we will convert to int32_t.
    */

    //ERROR CORRECTION: Latch width from 1-32
    if (width < 1) width = 1;
    if (width > 32) width = 32;

    uint32_t mask = 0;
    for (int i = 0; i < width; i++) {
        mask |= (1u << i); //load bits into mask (e.g. 5 bit width = ..0001 1111)
    }
    value &= mask; //extract lowest width bits of value
    if (value & (1u << (width - 1))) { //check if the most significant bit of the extracted field is 1
        value |= ~mask; //sign-extend the value by OR'ing all bits with a max unsigned value mask (padding with 1's)
    }

    
    return (int32_t)value; //convert to int32_t and return

}

