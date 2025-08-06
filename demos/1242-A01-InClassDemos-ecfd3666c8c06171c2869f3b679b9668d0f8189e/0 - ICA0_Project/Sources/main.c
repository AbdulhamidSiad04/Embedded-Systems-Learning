/********************************************************************/
// HC12 Program:  ICA0 - Sample Project
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        James Chaulk
// Details:       This project provides an example of a complete project.
//                It will be used to practice using git in ICA0.
// Date:          06 JAN 25
// Revision History :
//  each revision will have a date + desc. of changes
// 06 JAN 25 - Initial Commit


/********************************************************************/
// Library includes
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

//Other system includes or your includes go here
//#include <stdlib.h>
//#include <stdio.h>


/********************************************************************/
//Defines
/********************************************************************/

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
  //Any main local variables must be declared here

  // main entry point
  _DISABLE_COP();
  //EnableInterrupts;
  
/********************************************************************/
  // one-time initializations
/********************************************************************/

PT1AD1 &= 0b00011111;   //Turn off the LEDs
DDR01AD1 = 0b11100000;  //Set LED pins to be outputs
ATD1DIEN = 0b00011111;  //Enable digital inputs on switches

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    static unsigned counter = 0;

    while (++counter); // Example of a blocking delay
    PT1AD1 ^= 0b10000000;

  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
