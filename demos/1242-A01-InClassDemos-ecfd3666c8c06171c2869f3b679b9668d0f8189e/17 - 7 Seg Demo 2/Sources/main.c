/********************************************************************/
// HC12 Program:  7-Seg Animation Example
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James Chaulk
// Details:       This code provide an example of creating an animation
//                  on a 7-segment display
// Date:          April 4, 2025
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
#include "segs.h"
#include "sw_led.h"
#include "clock.h"
#include "rti.h"

/********************************************************************/
//Defines
/********************************************************************/

/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
volatile int newFrame = 0;  //A flag to indicate that a new frame should be displayed
/********************************************************************/
// Constants
/********************************************************************/
const char animation[] = { 0x80, 0xC0, 0xE0, 0xF0, 0xF1, 0xF9, 0xFB, 
                         0xBB, 0x9B, 0x8B, 0x8A, 0x82};  //Define the animation frames
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
Segs_Init();
SWL_Init();

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    static int frame = 0; //Selects which frame of the animation is being displayed
    static int address = 0; //Selects which 7-seg display is being used
    static SwState left, right = Idle;  //Button states

    Sw_ProcessD(&left, SWL_LEFT); //Update the button states
    Sw_ProcessD(&right, SWL_RIGHT);

    if (newFrame){  //If it's time to update the display
      frame = ++frame%12; //Advance to the next frame, but limit the values to 0 - 11
      Segs_Custom(address, animation[frame]); //Update the display
      newFrame = 0;   //Clear the flag
      
    }

    if(right == Pressed && address < 3) //If the right button is pressed, and the address isn't at the end of the top row
      Segs_ClearDigit(address++); //Clear the current digit and increment the address
      Segs_Custom(address, animation[frame]); //Display the current frame on the new digit
    
    if(left == Pressed && address > 0)  //If the left button is pressed and the address isn't at the end of the top row
      Segs_ClearDigit(address--); //Clear the current digit and decrement the address
      Segs_Custom(address, animation[frame]); //Display the current frame on the new digit
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

 if(!(rtiMasterCount%100))  //If the counter is a multiple of 100
  newFrame = 1; //Set the newFrame flag to True. 
 // Increment counters 
 rtiDelayCount--;
 rtiMasterCount++;

}