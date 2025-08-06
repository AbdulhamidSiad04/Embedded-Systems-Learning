/********************************************************************/
// HC12 Program:  Delay Examples
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        James Chaulk
// Details:       This program provides examples of blocking and non-blocking delays               
// Date:          28JAN25
// Revision History :
//  each revision will have a date + desc. of changes
// 28JAN25 - Initial creation in class
// 04FEB25 - Added comments and blocking delay examples



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
// Defines for LEDs and buttons
#define RED_LED 0b10000000
#define YELLOW_LED 0b01000000
#define GREEN_LED 0b00100000
#define BUTTONS 0b00011111

// Defines for #ifdef
//#define BLOCKING
#define NON_BLOCKING
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
PT1AD1 &= 0b00011111;
DDR01AD1 = 0b11100000;
ATD1DIEN = 0b00011111;

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    // This code block demonstrates blocking delays. If a counter hasn't reached its 
    // setpoint, the program gets trapped by the while loop until the setpoint is reached.
    // Execution of the rest of the super loop is `blocked` by each while loop. The push buttons
    // will somewhat unresponsive because they are only checked once per super loop iteration. 
    // The code needs to run all of the blocking delays before each button check.
    #ifdef BLOCKING
    long counter = 0; //Counters will reset to 0 at the start of each loop
    uint counter2 = 0;
    
    while(++counter < 250000); //Increment the counter until it is greater than 250000
    PT1AD1 ^= RED_LED;    //Toggle the LED

    //This delay will be very short, so the red and yellow LEDs will appear to turn on at the same time
    while(++counter2 < 5000); //Increment the counter until it is greater than 5000. 
    PT1AD1 ^= YELLOW_LED;     //Toggle the yellow 
    
    if (PT1AD1 & BUTTONS)   //If any of the push buttons are pressed
      PT1AD1 |= GREEN_LED;  //Turn on the green LED
    else                    //Otherwise,
      PT1AD1 &= ~GREEN_LED; //Turn the green LED off. 
    #endif

    // This code block demonstrates non-blocking delays. If a counter hasn't reached its 
    // setpoint, the program execution continues and more instructions can run. The push buttons
    // will be very responsive because there is very little time between each super loop iteration.
    // The code is constantly checking the state of the buttons.
    #ifdef NON_BLOCKING
    static long counter = 0; //These counters must be static so their values are retained
    static int counter2 = 0; //between loop iterations
    
    if(++counter > 250000){ //Increment the counter and check if it is greater than 250000
      PT1AD1 ^= RED_LED;  //Toggle the LED and reset the counter if it is
      counter = 0;
    }

    if(++counter2 > 5000){  //Increment another counter and check if it is greater than 5000
    PT1AD1 ^= YELLOW_LED;   //Toggle the LED and reset the counter if it is
    counter2 = 0;
    }
    
    if(PT1AD1 & BUTTONS)    //Check if any of the buttons are pressed
      PT1AD1 |= GREEN_LED;  //Turn on the LED if they are
    else                    //Otherwise,
      PT1AD1 &= ~GREEN_LED; //Turn the LED off
    #endif
    
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
