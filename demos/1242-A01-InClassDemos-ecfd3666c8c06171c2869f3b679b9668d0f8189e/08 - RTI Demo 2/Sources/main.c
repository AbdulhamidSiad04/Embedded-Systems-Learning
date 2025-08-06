/********************************************************************/
// HC12 Program:  RTI Example 2
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James Chaulk
// Details:       This demonstrates how to set up the ISR and use it 
//                to create non-blocking delays          
// Date:          04 Feb 25
// Revision History :
//  each revision will have a date + desc. of changes
//  11 MAR 25 - corrected bit shift in RTICTL configuration



/********************************************************************/
// Library includes
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

//Other system includes or your includes go here
//#include <stdlib.h>
//#include <stdio.h>

#include "sw_led.h"
#include "clock.h"

/********************************************************************/
//Defines
/********************************************************************/
#define ISR
/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
volatile unsigned long rtiCounter = 0; //Variables used in the ISR must be volatile

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
  EnableInterrupts; //Make sure interrupts have been enabled
  
/********************************************************************/
  // one-time initializations
/********************************************************************/
Clock_Set20MHZ(); //Set bus speed to 20MHz
SWL_Init();   //Initialize buttons and LEDs

//RTI Setup

//For this demo, the RTI will be set to interrupt every 2.5ms
// In your library, it will need to be set to interrupt every 1ms

//Step 1 - Set the RTICTL Register to set up the clock divider
RTICTL = RTICTL_RTDEC_MASK|(0b100<<4)|0b0001; //See 2.3.2.8
//              |               |       |_____RTR[3:0]
//              |               |_____________RTR[6:4]       
//              |_____________________________RTDEC
//

//Step 2 - Enable the RTI Interrupt
CRGINT |= CRGINT_RTIE_MASK; //See 2.3.2.5

//The RTI is now set up, 
//  but it will cause lots of problems if the ISR is not set up correctly

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    static SwState center;  //Create a button state variable
    Sw_Process(&center, SWL_CTR); //Update it at the start of each loop
    if(center == Pressed)         //If the center button is being pressed
      SWL_TOG(SWL_GREEN);         //Toggle the green LED
    if (!(rtiCounter % 10)) //The RTI has been set for 2.5ms, to the counter will be a multiple of 10 every 25ms
      SWL_TOG(SWL_YELLOW);  //This won't toggle every 25ms, measure it so see what it will do.
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
#ifdef ISR
//This is the declaration for the real-time interrupt service routine
interrupt VectorNumber_Vrti void Vrti_ISR(void) 
{
 CRGFLG = CRGFLG_RTIF_MASK; //Clear the RTI interrupt flag (2.3.2.4) & (13.5.3)

 rtiCounter++;  // Increment the master counter
 SWL_TOG(SWL_RED);  //Toggle the red LED every time the ISR runs
}
#endif