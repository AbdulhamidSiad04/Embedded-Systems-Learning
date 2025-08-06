/********************************************************************/
// HC12 Program:  Debounce_counter - Demonstration of different ways to read switches
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James C.
// Details:       This demo code can be used for testing the sw_led library               
// Date:          October 28, 2024
// Revision History :
//  each revision will have a date + desc. of changes
// Initial commit - October 28, 2024



/********************************************************************/
// Library includes
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

//Other system includes or your includes go here
//#include <stdlib.h>
//#include <stdio.h>

#include "rti.h"
#include "clock.h"
#include "sw_led.h"

/********************************************************************/
//Defines
/********************************************************************/
#define PLAIN_BAD
//#define NO_DEBOUNCE
//#define DEBOUNCE
//#define DEBOUNCE_STATES
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
  EnableInterrupts;
  
/********************************************************************/
  // one-time initializations
/********************************************************************/
Clock_Set20MHZ();
RTI_Init();
SWL_Init();

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    
    SwState up_state, down_state; //Create two SwState Variables
    
    //This code will increment the counter as fas as it can as long as a button is pressed
    #ifdef PLAIN_BAD
    if(SWL_Pushed(SWL_ANY))
      PT1AD1 += 32; //This will increment the 6th bit in PT1AD1
    #endif
    
    //This code adds a blocking delay to the counter will only increment once per press
    //If the button is quickly tapped, it may still increment 2 - 3 times. This is caused
    //by mechanical bouncing in the switch. 
    #ifdef NO_DEBOUNCE
    if(SWL_Pushed(SWL_ANY)){
      PT1AD1 += 32;
      while(SWL_Pushed(SWL_ANY)); //Hold the program until the button is released
    }
    #endif

    //This code adds the SWL_PushedDeb() function. It will read the buttons, wait a period of time,
    //then read the buttons again. If the two reads are the same, it is a valid button press and processed.
    //If the two reads are different, one of them was caused by a bouce so we start the process over.
    #ifdef DEBOUNCE
    if(SWL_PushedDeb(SWL_ANY)){ //Debounced button check
      PT1AD1 += 32;
      while(SWL_PushedDeb(SWL_ANY)); //We still need a blocking delay to hold the program until the button is released
    }
    #endif

    //This code adds a button states to avoid the need for a blocking delay. The state flow diagram was explained in class.
    //The general flow is Idle -> Pressed -> Held -> Released -> Idle. I suggest implementing it with a switch statement.
    //Because there are no blocking delays, the down button can be read and acted upon while the up button is held. 
    #ifdef DEBOUNCE_STATES
      //Pass the Sw_ProcessD() function a pointer to a SwState variable and a button position. 
      //This allows the state to be directly updated in the function. The current state is returned and can be compared to 
      //a SwState enumerator value
      if(Sw_ProcessD(&up_state, SWL_UP) == Pressed) 
      PT1AD1 += 32;
      
      if(Sw_ProcessD(&down_state, SWL_DOWN) == Pressed)
      PT1AD1 -= 32;
    
    #endif
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/

interrupt VectorNumber_Vrti void Vrti_ISR(void)
{
 CRGFLG = CRGFLG_RTIF_MASK; //clear flag;

 // Increment counters 
 rtiDelayCount++;
 rtiMasterCount++;

}