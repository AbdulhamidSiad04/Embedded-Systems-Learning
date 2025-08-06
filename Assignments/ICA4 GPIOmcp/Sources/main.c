/********************************************************************/
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     8MHz
// Author:        Abdulhamid Siad
// Details:      Implementing GPIO and Using AD2 To measure Frequencies            
// Date:          25/07/31
// Revision History :
//  each revision will have a date + desc. of changes



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
  //Any main local variables must be declared here

  // main entry point
  _DISABLE_COP();
  //EnableInterrupts;
  
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
      unsigned long counter = 0;
    PT1AD1 ^= Green_Led;
    // this is called blocking delay, will be trapped in the loop, 
    // so the microcontrolled is not able to execute any other command
    while(counter < 222)
    {
      // will want to gratually increase to reach 500000 so increment counter
      counter++;
    }
    //if call counter =0 inside of {} while counter<500000 condition
    //will always be true because it will never reach 500000 because 
    //Each time we increase we reset so it never leaves 1 really.
    //That why we have it outside of {} so then we can reset it,means
    //It will only execurte when the loop finishes so condition == false(what we need)
    //because Counter has now reached 500000

    //If not reset will get blurry instead of steady blink because main runs forever
    //each new led toggle needs 500000 count pause of program, so when first delay finished
    //already sitting at 500000 and don't reset, then counter condition is already false
    //so loop gets skipped and led keeps flipping almost instantly.
    //Reseting just guarantees every cycle gets fresh start of 0 and leds keeos rhythm
    counter =0;

    /*---------------------------------------------------------------
  Delay-calculation notes  (for 500 Hz requirement)

  Measured with unsigned long counter and loop limit = 1000
      Full LED period (high + low)  T ≈ 9 ms   →  two delays
      Single delay (one high *or* low)          = 4.5 ms

  Current loop limit (Ncurrent)                 = 1000
  Time per iteration:
      t_iter = 4.5 ms / 1000  ≈ 4.5 µs

  To hit a 1 ms delay (half of a 2 ms, 500 Hz square-wave):
      Nnew  = 1 ms / 4.5 µs  ≈ 222.2  →  use 222

  ⇒  while(counter < 222) { counter++; }   // gives ≈500 Hz
----------------------------------------------------------------*/

  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
