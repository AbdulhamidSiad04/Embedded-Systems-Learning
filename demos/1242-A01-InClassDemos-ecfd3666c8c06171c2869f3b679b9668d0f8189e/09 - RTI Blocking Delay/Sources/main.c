/********************************************************************/
// HC12 Program:  RTI Blocking Delays
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James Chaulk
// Details:       This demonstrates how to create blocking delay with the RTI
// Date:          04 Feb 25
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

#include "rti.h"
#include "sw_led.h"
#include "clock.h"

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
  EnableInterrupts; //Make sure interrupts have been enabled
  
/********************************************************************/
  // one-time initializations
/********************************************************************/
Clock_Set20MHZ(); //Set bus speed to 20MHz
RTI_Init();   //Initialize real-time interrupt to 1ms
SWL_Init();   //Initialize buttons and LEDs

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    static SwState center;  //Create a button state variable
    Sw_Process(&center, SWL_CTR); //Update it at the start of each loop
    
    rtiDelayCount = 2000; //Set the delay counter to some value
    while(rtiDelayCount); //It will be decremented in the ISR
    SWL_TOG(SWL_YELLOW);  //Do some action when the delay is over

    //Unfortunately, this means that my button state is only being updated once every 2 seconds
    //This long delay can create confusing states as the real switch is out of sync with the software state
    if(center == Pressed)         //If the center button is being pressed
      SWL_TOG(SWL_GREEN);         //Toggle the green LED
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/

//This is the declaration for the real-time interrupt service routine
interrupt VectorNumber_Vrti void Vrti_ISR(void) 
{
 CRGFLG = CRGFLG_RTIF_MASK; //Clear the RTI interrupt flag (2.3.2.4) & (13.5.3)

 rtiMasterCount++;  // Increment the master counter
 rtiDelayCount--;
 SWL_TOG(SWL_RED);  //Toggle the red LED every time the ISR runs
 
}