/********************************************************************/
// HC12 Program:  RTI Example 1
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James Chaulk
// Details:       This program serves as a proof that the RTI allows
//                  code to be executed outside of the usual program
//                  flow. The ISR will execute without being called 
//                  in software.                
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
//Clock_Set2MHZ(); //Set bus speed to 2MHz. The RTI will not change with a slower clock
//Clock_Set40MHZ(); //Set bus speed to 40MHz. The RTI will not change with a faster clock
RTI_Init();   //Initialize real-time interrupt to 1ms
SWL_Init();   //Initialize buttons and LEDs

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    //The main loop is empty.
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
 SWL_TOG(SWL_RED);  //Toggle the red LED every time the ISR runs
 
}