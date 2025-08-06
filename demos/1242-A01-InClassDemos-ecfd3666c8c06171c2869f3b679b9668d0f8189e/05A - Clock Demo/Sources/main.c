/********************************************************************/
// HC12 Program:  Clock Demo - Demonstrates setting clock speed
// Processor:     MC9S12XDP512
// Bus Speed:     8 - 24 MHz
// Author:        James Chaulk
// Details:       This program demonstrates setting the clock speed and 
//                providing it on pin 39 of the micro for measurement. 
//                Your library may not work with the button presses. That 
//                is okay. You wouldn't normally change the clock speed after
//                program initialization. 
// Date:          7 October 2024
// Revision History :
//  each revision will have a date + desc. of changes
// 25 February 2025 - Added expanded clock setting in initializations
//                    Updated to use switch states



/********************************************************************/
// Library includes
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "sw_led.h"
#include "clock.h"


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


//CLKSEL_PLLSEL = 0;  //SYNR and REFDV can only be modified when PLLSEL = 0 (2.3.2.1)
CLKSEL &= ~CLKSEL_PLLSEL_MASK;  //Disable PPL. SYNR and REFDV can only be modified when PLLSEL = 0 (2.3.2.1)

/*Set PLL to 37MHz, therefore clock to 18.5MHZ*/
SYNR = 36; 
REFDV = 31;

//PLLCTL_PLLON = 1; //Ensure PLL is turned on (2.3.2.7)
PLLCTL |= PLLCTL_AUTO_MASK | PLLCTL_PLLON_MASK; //PLL auto and PLL ON

//while (!CRGFLG_LOCK); //Wait for PLL to stabilize (2.3.2.4)
while(!(CRGFLG & CRGFLG_LOCK_MASK)); //Wait till PLL is locked into the frequency

CLKSEL |= CLKSEL_PSTP_MASK | CLKSEL_PLLSEL_MASK; // Enable pseudo stop to reduce wear on crystal 
                                                 // Use PLLCLK for system clocks (2.3.2.6)

SWL_Init();
//Clock_EnableOutput(4); //Enable the clock output on pin 39.
                       //Divide the clock by 4 for easier measurement with the AD2

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    //Set up a counter based blocking delay to toggle an LED
    uint Counter = 0;
    SwState left, center, right;
    
    Sw_Process(&left, SWL_LEFT);
    Sw_Process(&center, SWL_CTR);
    Sw_Process(&right, SWL_RIGHT);
    
    while (++Counter < 15000);
    PT1AD1 ^= 0b10000000; 
    
    if (right == Pressed){
      Clock_Set40MHZ(); //If the right button is pushed, set the clock to 40MHz
    }
    if (center == Pressed){
      Clock_Set20MHZ(); //If the center button is pushed, set the clock to 20MHz
    }
    if (left == Pressed){
      Clock_Set8MHZ(); //If the left button is pushed, set the clock to 8MHz
    }   
  }                
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
