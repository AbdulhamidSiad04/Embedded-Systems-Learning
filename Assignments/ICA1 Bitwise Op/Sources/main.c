/********************************************************************/
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     8MHz
// Author:        Abdulhamid Siad
// Details:       Impelementing Bitwise Operations
// Date:          25/7/29
// Revision History :
//  each revision will have a date + desc. of changes

/********************************************************************/
// Library includes
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

// Other system includes or your includes go here
// #include <stdlib.h>
// #include <stdio.h>

/********************************************************************/
// Defines
/********************************************************************/
#define Red_Led 0b10000000
#define Yellow_Led 0b01000000
#define Green_Led 0b00100000
#define CTR_Button 0b00000001
/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/

/********************************************************************/
// Constants
/********************************************************************/

/********************************************************************/
// Main Entry
/********************************************************************/
void main(void)
{
  // Any main local variables must be declared here

  // main entry point
  _DISABLE_COP();
  // EnableInterrupts;

  /********************************************************************/
  // one-time initializations
  /********************************************************************/
  PT1AD1 &= 0b00011111;  // Turn off LEDs before enabling outputs
  DDR01AD1 = 0b11100000; // Set LED pins to be outputs
  ATD1DIEN = 0b00011111; // Disable A-to-D button pins (22.3.2.69)

  
  /********************************************************************/
  // main program loop
  /********************************************************************/
  
  for (;;)
  {
    PT1AD1 ^= Green_Led;

    // static unsigned long delay = 0;
    // static unsigned char counter = 0;
    // if (++delay == 100000)
    // {            // When delay has incremented 100000 times
    //   counter++; // Increment counter
    //   delay = 0; // Reset delay timer
    // }
    // // We could use the LEDs to display the value in binary, but PT1AD1 = counter; would display
    // // the most significant bits (MSBs) of the counter variable on the LEDs. We want to see the LSBs on
    // // the LEDs.
    // //Ex: 00000111 so shift by 5 left then 3 LSB will be 11100000 which are the leds we want to show
    // PT1AD1 = counter << 5;
  }
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
