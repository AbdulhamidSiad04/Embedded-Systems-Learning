/********************************************************************/
// HC12 Program:  Library Example
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        James Chaulk
// Details:       This program provides several examples of using the sw_led library              
// Date:          04FEB25
// Revision History :
//  each revision will have a date + desc. of changes
// 04FEB25 - Initial Commit



/********************************************************************/
// Library includes
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

//Other system includes or your includes go here
//#include <stdlib.h>
//#include <stdio.h>

#include "sw_led.h" //Include the switch and LED library


/********************************************************************/
//Defines
/********************************************************************/

//#define LEDS
#define BUTTONS

/********************************************************************/
// Local Prototypes
/********************************************************************/
void led_counter(); //A function to create a binary counter on the LEDs

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
SWL_Init(); //Initialize the pushbuttons and LEDs

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    // This code block will test some of the LED functions from the sw_led library
    #ifdef LEDS
    SWL_ON(SWL_RED); //This will turn on the red LED
    SWL_TOG(SWL_ALL); //This will toggle all of the LEDs
    SWL_OFF(SWL_YELLOW|SWL_GREEN); //This will turn off the green and yellow LEDs
    #endif

    //This code block will test some of the pushbutton functions from the sw_led library
    #ifdef BUTTONS
    if(SWL_Any()){  //If any of the buttons are being pressed
      led_counter(); //Increment a counter on the LEDs
    }
    while (SWL_Pushed(SWL_UP));  //Block execution until the center button is released
    #endif
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void led_counter(void){
  static unsigned char counter = 0;
  PT1AD1 = ++counter<<5;
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
