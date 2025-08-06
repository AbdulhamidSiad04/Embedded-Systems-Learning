/********************************************************************/
// HC12 Program:  PLL Demo 2
// Processor:     MC9S12XDP512
// Bus Speed:     4 MHz
// Author:        James Chaulk
// Details:       This program uses the PLL to create a 4MHz bus speed.
//                  The bus clock will be divided by 4 and output on pin 39 of the micro
//                  The main loop uses a blocking delay to create a binary counter on the LEDs
//                  Detailed information on PLL operation can be count in section 2.4.1.1 of the datasheet.
//                  This code is representative of what detailed library comments should look like
// Date:          28 FEB 25
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

#include "sw_led.h" //Included for LED functions

/********************************************************************/
//Defines
/********************************************************************/

/********************************************************************/
// Local Prototypes
/********************************************************************/
void Clock_Set4MHZ(void);
void Enable_CLK_Output(int);

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
Clock_Set4MHZ();  //Set the bus speed to 4MHz 
Enable_CLK_Output(4);  // Divide the bus clock by 4 and output on Pin 39
SWL_Init();       // Initialize Port AD1 to use the LEDs

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    unsigned long delay = 100000; //Preset the delay to some value. It will be reset on every loop
    static unsigned int counter = 0;  //Create a counter variable that will retain it's value between loops
    while (--delay);  //Blocking delay that will block until delay has decremented to 0
    PT1AD1 = (PT1AD1 & ~SWL_ALL) | (++counter << 5); //Increment counter, left-shift 5 bits, and display the result on the PT1AD1 LEDs
                                                     // while leaving the rest of PT1AD1 unchanged.

  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void Clock_Set4MHZ(void){
  CLKSEL &= ~CLKSEL_PLLSEL_MASK;  //Disable PLL. SYNR and REFDV can only be modified when PLLSEL = 0 (2.3.2.1)

// See datasheet 2.3.2.1 + 2.3.2.2 for details on math below
// Bus Clock = PLLCLK/2
// PLLCLK = 2*OSCLK*[(SYNR+1)/(REFDV+1)]
// PLLCLK = 32MHz*[(SYNR+1)/(REFDV+1)] -> Want 8MHz PLL Clock for 4MHz Bus
// 8MHz/32MHz = 0.25 = (SYNR+1)/(REFDV+1)
// 1/4 = 0.25 -> SYNR = 0, REFDV = 3
//
// Can also use SYNR = 7, REFDV = 31 because
// 8MHz/32MHz = (SYNR+1)/(REFDV+1) = (7+1)/(31+1)00

SYNR = 7; //Set the Synthesizer Register to 7
REFDV = 31; //Set the Reference Divider to 31

PLLCTL |= PLLCTL_AUTO_MASK | PLLCTL_PLLON_MASK; //Enable PLL auto and PLL ON (2.3.2.7)
while(!(CRGFLG & CRGFLG_LOCK_MASK)); //Wait till PLL is locked into the frequency(2.3.2.4)
CLKSEL |= CLKSEL_PSTP_MASK | CLKSEL_PLLSEL_MASK; // Enable pseudo stop to reduce wear on crystal 
                                                 // Use PLLCLK for system clocks (2.3.2.6)

}

void Enable_CLK_Output(int div){
// Clock output options are described in datasheet 22.3.2.13
// ECLK is on pin 39 in 112-Pin LQFP package (1.2.1)

div -= 1;   // divider of 1 = 00, divider of 2 = 01, etc. (22.3.2.13)    

ECLKCTL_NECLK = 1; //Disable clock output while making adjustments (1.2.1 + 22.3.2.13)
ECLKCTL &= ~(ECLKCTL_EDIV1_MASK|ECLKCTL_EDIV0_MASK); //Clear divider bits (22.3.2.13)
ECLKCTL |= div; //Set the output divider bits
ECLKCTL &= ~ECLKCTL_NECLK_MASK; //Enable Clock Output on Pin 39 by clearing NECLK bit(1.2.1 + 22.3.2.13)
}

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
